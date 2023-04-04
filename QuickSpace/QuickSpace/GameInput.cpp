#include "stdafx.h"
#include "GameInput.h"

#include "GameRoot.h"

namespace QuickSpace
{
	InputGroup& GameInput::Left()
	{
		return m_left;
	}

	InputGroup& GameInput::Right()
	{
		return m_right;
	}

	InputGroup& GameInput::Up()
	{
		return m_up;
	}

	InputGroup& GameInput::Down()
	{
		return m_down;
	}

	InputGroup& GameInput::Ok()
	{
		return m_ok;
	}

	GameInput& GameInput::Instance()
	{
		return GameRoot::Global().GetInput();
	}
}
