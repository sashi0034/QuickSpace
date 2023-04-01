#include "../../stdafx.h"
#include "Demos.h"
#include "../ActorManager.h"
#include "../CoroUtil.h"
#include "../GameRoot.h"

void QuickSpace::Demos::InitDemos()
{
	GameRoot::Global().GetActorManager().Birth(new Demo4());
	GameRoot::Global().GetActorManager().BirthAs<Demo1>(new Demo1());
	GameRoot::Global().GetActorManager().Birth(new Demo2());
	GameRoot::Global().GetActorManager().Birth(new Demo3());
}

void QuickSpace::Demos::Demo1::TestPrint()
{
	Print(U"test Demo1");
}

void QuickSpace::Demos::Demo1::Update()
{
	// テクスチャを描く | Draw the texture
	texture.draw(20, 20);

}

QuickSpace::Demos::Demo2::Demo2()
{
	font.addFallback(emojiFont);
}

void QuickSpace::Demos::Demo2::Update()
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


QuickSpace::Demos::Demo3::Demo3()
{
	m_task = GameRoot::Global().GetCoroutineManager().Start([&](auto&& yield){TestCoro1(yield, 12); });
	GameRoot::Global().GetCoroutineManager().Start([&](auto&& yield){TestCoro2(yield); });
}

void QuickSpace::Demos::Demo3::Update()
{}

QuickSpace::CoroTask QuickSpace::Demos::Demo3::TestCoro1(CoroTaskYield& yield, int count)
{
	for (int i=0; i<count; ++i)
	{
		Print(U"count: {}"_fmt(i));
		CoroUtil::WaitForTime(yield, 0.5);
	}
}

QuickSpace::CoroTask QuickSpace::Demos::Demo3::TestCoro2(CoroTaskYield& yield)
{
	CoroUtil::WaitForCoro(yield, m_task);
	Print(U"finished task");
}

void QuickSpace::Demos::Demo4::Update()
{
	const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

	const uint64 t = Time::GetMillisec();
	const int32 n = (t / 250 % 4);

	auto&& texture = GameAsset::Instance().pondelion_32x32;

	(void)texture(32 * n, 0, 32, 32)
		.scaled(2).draw(100, 100);

	const ScopedCustomShader2D shader{ ps };
	(void)texture(32 * n, 0, 32, 32)
		.scaled(3).draw(100, 200);
}

float QuickSpace::Demos::Demo4::OrderPriority()
{
	return 100.0f;
}
