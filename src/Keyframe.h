#pragma once

#include "InterpolationMode.h"

struct KeyDataPoint
{
	float x;
	float y;
};

class Keyframe
{
public:
	static float getValue(InterpolationMode mode, const Keyframe& a, const Keyframe& b, float time);

	Keyframe(
		const KeyDataPoint& controlPoint,
		const KeyDataPoint& leftHandle,
		const KeyDataPoint& rightHandle) :
		m_controlPoint(controlPoint),
		m_leftHandle(leftHandle),
		m_rightHandle(rightHandle)
	{
	}

	Keyframe() : Keyframe({ 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f })
	{
	}

	const KeyDataPoint& getControlPoint() const { return m_controlPoint; }
	const KeyDataPoint& getLeftHandle() const { return m_leftHandle; }
	const KeyDataPoint& getRightHandle() const { return m_rightHandle; }
	
private:
	KeyDataPoint m_controlPoint;
	KeyDataPoint m_leftHandle;
	KeyDataPoint m_rightHandle;

	static float getValueBezier(const Keyframe& a, const Keyframe& b, float time);
	static float getValueLinear(const Keyframe& a, const Keyframe& b, float time);
	static double cardano(const KeyDataPoint curve[], float time);
	static double bezier(double t, double a, double b, double c, double d);
	static double minNonNegative(double a, double b);
};
