﻿#pragma once

namespace QuickSpace
{
	class GameInput
	{
	public:
		InputGroup& Left();
		InputGroup& Right();
		InputGroup& Up();
		InputGroup& Down();
		InputGroup& Ok();

		static GameInput& Instance();
	private:
		InputGroup m_left = KeyLeft | KeyA;
		InputGroup m_right = KeyRight | KeyD;
		InputGroup m_up = KeyUp | KeyW;
		InputGroup m_down = KeyDown | KeyS;
		InputGroup m_ok = KeySpace;
	};
}
