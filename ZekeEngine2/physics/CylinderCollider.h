#pragma once
class CylinderCollider : ICollider
{
public:
	~CylinderCollider() {
		delete m_shape;
	}
	void Create(float radius, float height) {
		m_shape = new btCylinderShape({ radius,height,radius });
		m_height = height;
		m_radius = radius;
	}
	btCollisionShape* GetBody() {
		return m_shape;
	}
private:
	btCylinderShape* m_shape = nullptr;
	float m_radius = 0.f;
	float m_height = 0.f;
};

