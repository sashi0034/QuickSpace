#pragma once
#include "SepEdge.h"
#include "TerrEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	enum class EPlayerState
	{
		Moving,
		Drawing,
	};

	class Player : public ActorBase
	{
	public:
		Player();
		void Init();
		void Update() override;
		float OrderPriority() override;
		TerrEdgeRef& GetEdgeTarget();

		Float2 EdgeCursor() const;
		bool IsMovingCursorNow() const;
		Angle GetAngle() const;
	private:
		class PlayerAction;

		EPlayerState m_state = EPlayerState::Moving;
		TerrEdgeRef m_edgeTarget{}; // 接触中の辺
		Float2 m_edgeCursor{}; // プレイヤーのカーソル
		Float2 m_edgeCursorBefore{};
		Angle m_angle{};
		Array<SepEdge> m_drawnEdges{}; // 現在描画中の辺

		Point roundEdgeCursor() const;
		float getSpeed() const;

		float m_animValue{};

		void moveOnEdge();
		void changeEdgeTargetAutoAfterMoved();
		void moveWithDraw();
		bool canRotateDrawingDirection(EAngle angle);
		bool checkStartDrawing(EAngle angle);
		void startDrawing(EAngle angle);
		void continueDrawing(EAngle angle, const TerrVertex& oldEnd);
		void rotateDrawingDirection(EAngle angle);
		void checkFinishDrawing();
		void finishDrawing(Point intersectedPoint);
		void extendDrawingEdge(EAngle direction);
		void checkStickDrawingEdgeToFrontier(const EAngle direction, SepEdge cursorExtendedEdge);
		void checkMoveIntersect(EAngle angle, float speed, bool isHorizontal);
		static InputGroup& inputAngle(EAngle angle);
	};
}
