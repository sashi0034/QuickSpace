﻿#include "../stdafx.h"
#include "Demos.h"
#include "ActorManager.h"

void QuickSpace::Demos::InitDemos()
{
	ActorManager::Global().BirthAs<Demo1>(new Demo1());
	ActorManager::Global().Birth(new Demo2());
}

void QuickSpace::Demo1::TestPrint()
{
	Print(U"test Demo1");
}

void QuickSpace::Demo1::Update()
{
	// テクスチャを描く | Draw the texture
	texture.draw(20, 20);

}

QuickSpace::Demo2::Demo2()
{
	font.addFallback(emojiFont);
}

void QuickSpace::Demo2::Update()
{

	// テキストを描く | Draw text
	font(U"Hello, Siv3D!🎮").draw(64, Vec2{ 20, 340 }, ColorF{ 0.2, 0.4, 0.8 });

	// 指定した範囲内にテキストを描く | Draw text within a specified area
	font(U"Siv3D (シブスリーディー) は、ゲームやアプリを楽しく簡単な C++ コードで開発できるフレームワークです。")
		.draw(18, Rect{ 20, 430, 480, 200 }, Palette::Black);

	// 長方形を描く | Draw a rectangle
	Rect{ 540, 20, 80, 80 }.draw();

	// 角丸長方形を描く | Draw a rounded rectangle
	RoundRect{ 680, 20, 80, 200, 20 }.draw(ColorF{ 0.0, 0.4, 0.6 });

	// 円を描く | Draw a circle
	Circle{ 580, 180, 40 }.draw(Palette::Seagreen);

	// 矢印を描く | Draw an arrow
	Line{ 540, 330, 760, 260 }.drawArrow(8, SizeF{ 20, 20 }, ColorF{ 0.4 });

	// 半透明の円を描く | Draw a semi-transparent circle
	Circle{ Cursor::Pos(), 40 }.draw(ColorF{ 1.0, 0.0, 0.0, 0.5 });

	// ボタン | Button
	if (SimpleGUI::Button(U"count: {}"_fmt(count), Vec2{ 520, 370 }, 120, (checked == false)))
	{
		// カウントを増やす | Increase the count
		++count;
	}

	// チェックボックス | Checkbox
	SimpleGUI::CheckBox(checked, U"Lock \U000F033E", Vec2{ 660, 370 }, 120);

	// スライダー | Slider
	SimpleGUI::Slider(U"speed: {:.1f}"_fmt(speed), speed, 100, 400, Vec2{ 520, 420 }, 140, 120);

	// 左キーが押されていたら | If left key is pressed
	if (KeyLeft.pressed())
	{
		// プレイヤーが左に移動する | Player moves left
		playerPosX = Max((playerPosX - speed * Scene::DeltaTime()), 60.0);
		isPlayerFacingRight = false;
	}

	// 右キーが押されていたら | If right key is pressed
	if (KeyRight.pressed())
	{
		// プレイヤーが右に移動する | Player moves right
		playerPosX = Min((playerPosX + speed * Scene::DeltaTime()), 740.0);
		isPlayerFacingRight = true;
	}

	// プレイヤーを描く | Draw the player
	emoji.scaled(0.75).mirrored(isPlayerFacingRight).drawAt(playerPosX, 540);
}