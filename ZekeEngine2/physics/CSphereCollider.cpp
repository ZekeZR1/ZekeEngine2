#include "stdafx.h"
#include "CSphereCollider.h"



CSphereCollider::CSphereCollider() :
	shape(NULL)
{
}

CSphereCollider::~CSphereCollider()
{
	delete shape;
}

void CSphereCollider::Create(const float radius)
{
	shape = new btSphereShape(radius);
}