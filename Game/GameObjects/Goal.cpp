#include "stdafx.h"
#include "Goal.h"
#include "Ball.h"
#include "Car.h"
#include "ScoreManager.h"

bool Goal::Start() {
	//TODO : ゴースト使ってゴール判定する
	/*m_ghost = new btGhostObject();
	btVector3 shapee(1, 1, 1);
	m_boxShape = new btBoxShape(shapee);
	m_ghost->setCollisionShape(m_boxShape);
	btTransform tr;
	tr.setOrigin(btVector3(0,0,0));
	m_ghost->setWorldTransform(tr);
	btGhostPairCallback cb;
	PhysicsWorld().GetDynamicWorld()->addCollisionObject(m_ghost);*/
	return true;
}

void Goal::OnDestroy() {
	//delete m_ghost;
	//delete m_boxShape;
}

void Goal::Update() {
	//if (!m_isHost) return;

	//if (mp_ball == nullptr) {
	//	mp_ball = FindGO<Ball>("BallChan");
	//}
	//else {
	//	auto pos = mp_ball->GetPosition();
	//	if (pos.z <= -175 or pos.z >= 175) {
	//		auto manager = FindGO<ScoreManager>("ScoreManager");
	//		if (pos.z < 0) {
	//			manager->Goal(ScoreManager::enOrangeTeam);
	//		}
	//		if (pos.z > 0)
	//		{
	//			manager->Goal(ScoreManager::enBlueTeam);
	//		}
	//		printf("Goal");
	//		m_isScored = true;
	//	}
	//}
}
