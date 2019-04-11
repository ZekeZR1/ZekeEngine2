#pragma once
class CSphereCollider
{
public:
	CSphereCollider();
	~CSphereCollider();
	void Create(const float radius);
	btCollisionShape* GetBody() 
	{
		return shape;
	}
private:
	btSphereShape * shape;
};

