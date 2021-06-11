#pragma once

#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "EditorState.h"

class Actor
{
public:
	Actor(EditorState& editorState, const std::string& name) :
		m_editorState(editorState),
		m_name(name),
		m_selectable(false)
	{
	}

	const std::string& getName() const { return m_name; }

	virtual void init(const std::string& resourceDirectory) { }
	virtual void update(float deltaTime) { }
	virtual void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime) { }

protected:
	EditorState& getEditorState() const { return m_editorState; }

private:
	EditorState& m_editorState;
	std::string m_name;

	bool m_selectable;
};

