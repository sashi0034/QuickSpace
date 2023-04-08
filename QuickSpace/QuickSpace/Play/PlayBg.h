#pragma once
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class PlayBg : public ActorBase
	{
	public:
		PlayBg();
		void Update() override;
	private:
		void updateScript();
		DirectoryWatcher m_scriptDirWatcher{U"asset/script"};
		Script m_script{U"asset/script/Bg1.as"};
	};
}
