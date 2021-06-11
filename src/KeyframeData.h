#pragma once

#include <glm/gtc/type_ptr.hpp>

class KeyframeData
{
public:
	KeyframeData(glm::vec3 position, glm::vec3 rotation) :
		m_position(position),
		m_rotation(rotation),
		m_clipStart(false),
		m_keyframeId(-1),
		m_clipId(-1)
	{
	}

	glm::vec3 getPosition() const { return m_position; }
	glm::vec3 getRotation() const { return m_rotation; }

	bool getClipStart() const { return m_clipStart; }
	void setClipStart(bool clipStart) { m_clipStart = clipStart; }

	int getKeyframeId() const { return m_keyframeId; }
	void setKeyframeId(int keyframeId) { m_keyframeId = keyframeId; }

	int getClipId() const { return m_clipId; }
	void setClipId(int clipId) { m_clipId = clipId; }

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	bool m_clipStart;

	int m_keyframeId;
	int m_clipId;
};

