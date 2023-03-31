﻿#pragma once
#include "Actor.h"
#include "CoroutineManager.h"

namespace QuickSpace
{
	namespace Demos
	{
		void InitDemos();
	}

	class Demo1 : public IActor
	{
		// 画像ファイルからテクスチャを作成する | Create a texture from an image file
		const Texture texture{ U"example/windmill.png" };
	public:
		void TestPrint();
		void Update() override;
	};

	class Demo2 : public IActor
	{
		// 絵文字からテクスチャを作成する | Create a texture from an emoji
		const Texture emoji{ U"🦖"_emoji };

		// 太文字のフォントを作成する | Create a bold font with MSDF method
		const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

		// テキストに含まれる絵文字のためのフォントを作成し、font に追加する | Create a font for emojis in text and add it to font as a fallback
		const Font emojiFont{ 48, Typeface::ColorEmoji };

		// ボタンを押した回数 | Number of button presses
		int32 count = 0;

		// チェックボックスの状態 | Checkbox state
		bool checked = false;

		// プレイヤーの移動スピード | Player's movement speed
		double speed = 200.0;

		// プレイヤーの X 座標 | Player's X position
		double playerPosX = 400;

		// プレイヤーが右を向いているか | Whether player is facing right
		bool isPlayerFacingRight = true;
	public:
		Demo2();
		void Update() override;
	};

	class Demo3 : public IActor
	{
	public:
		Demo3();
		void Update() override;
		CoroTask TestCoro1(CoroTaskYield& yield, int count);
		CoroTask TestCoro2(CoroTaskYield& yield);
	private:
		std::shared_ptr<CoroutineElement> m_task;
	};
}
