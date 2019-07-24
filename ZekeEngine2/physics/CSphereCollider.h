#pragma once
class CSphereCollider : public ICollider
{
public:
	CSphereCollider();
	~CSphereCollider();
	void Create(const float radius);
	btCollisionShape* GetBody() const override
	{
		return shape;
	}
private:
	btSphereShape * shape;
};

