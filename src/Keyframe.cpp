#include "Keyframe.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <glm/gtc/type_ptr.hpp>

float Keyframe::getValue(InterpolationMode mode, const Keyframe& a, const Keyframe& b, float time)
{
	switch (mode)
	{
	case InterpolationMode::BEZIER:
		return getValueBezier(a, b, time);
	case InterpolationMode::LINEAR:
		return getValueLinear(a, b, time);
	default:
		return 0.0f;
	}
}

float Keyframe::getValueBezier(const Keyframe& a, const Keyframe& b, float time)
{
	KeyDataPoint curve[4] =
	{
		a.m_controlPoint,
		a.m_rightHandle,
		b.m_leftHandle,
		b.m_controlPoint,
	};

	// TODO: Does this work well with single-precision?

	double root = cardano(curve, time);

	return (float)bezier(root, (double)curve[0].y, (double)curve[1].y, (double)curve[2].y, (double)curve[3].y);
}

float Keyframe::getValueLinear(const Keyframe& a, const Keyframe& b, float time)
{
	const auto& controlA = a.getControlPoint();
	const auto& controlB = b.getControlPoint();

	float amount = (time - controlA.x) / (controlB.x - controlA.x);

	return controlA.y * (1.0f - amount) + controlB.y * amount;
}

double Keyframe::cardano(const KeyDataPoint curve[], float time)
{
	KeyDataPoint aligned[4] = {};

	for (size_t i = 0; i < 4; i++)
	{
		aligned[i] = { -curve[i].y, curve[i].x - time };
	}

	double pa = (double)aligned[0].y;
	double pb = (double)aligned[1].y;
	double pc = (double)aligned[2].y;
	double pd = (double)aligned[3].y;

	double d = (-pa + 3.0 * pb - 3.0 * pc + pd);

	if (d == 0.0)
	{
		d = 0.00001; // Some epsilon to avoid divide-by-zero.
	}

	double a = (3.0 * pa - 6.0 * pb + 3.0 * pc) / d;
	double b = (-3.0 * pa + 3.0 * pb) / d;
	double c = pa / d;
	
	double p = (3.0 * b - a * a) / 3.0;
	double p3 = p / 3.0;
	double q = (2.0 * a * a * a - 9.0 * a * b + 27.0 * c) / 27.0;
	double q2 = q / 2.0;
	
	double discriminant = q2 * q2 + p3 * p3 * p3;

	if (discriminant < 0.0)
	{
		double mp3 = -p / 3.0;
		double mp33 = mp3 * mp3 * mp3;
		double r = std::sqrt(mp33);
		
		double t = -q / (2.0 * r);
		double cosphi = t < -1.0 ? -1.0 : t > 1.0 ? 1.0 : t;
		double phi = std::acos(cosphi);
		double crtr = std::cbrt(r);
		double t1 = 2.0 * crtr;
		double x1 = t1 * std::cos(phi / 3.0) - a / 3.0;
		double x2 = t1 * std::cos((phi + glm::pi<double>() * 2.0) / 3.0) - a / 3.0;
		double x3 = t1 * std::cos((phi + 2 * glm::pi<double>() * 2.0) / 3.0) - a / 3.0;

		return minNonNegative(x1, minNonNegative(x2, x3));
	}
	else if (discriminant == 0.0f)
	{
		double u1 = q2 < 0.0 ? std::cbrt(-q2) : -std::cbrt(q2);
		double x1 = 2.0 * u1 - a / 3.0;
		double x2 = -u1 - a / 3.0;

		return minNonNegative(x1, x2);
	}
	else
	{
		double sd = std::sqrt(discriminant);
		double u1 = std::cbrt(-q2 + sd);
		double v1 = std::cbrt(q2 + sd);
		double x1 = u1 - v1 - a / 3.0;

		return x1;
	}
}

double Keyframe::bezier(double t, double a, double b, double c, double d)
{
	return
		a * std::pow(1.0 - t, 3.0) +
		b * 3.0 * t * std::pow(1.0 - t, 2.0) +
		c * 3.0 * std::pow(t, 2.0) * (1.0 - t) +
		d * std::pow(t, 3.0);
}

double Keyframe::minNonNegative(double a, double b)
{
	if (b < a)
	{
		std::swap(a, b);
	}

	if (a < 0.0)
	{
		if (b < 0.0)
		{
			return std::numeric_limits<double>::max();
		}

		return b;
	}

	return a;
}

