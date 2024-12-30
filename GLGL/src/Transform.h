#pragma once

#include "MathUtil.h"

class Transform {
public:
	Transform(){}

	void SetScale(float scale);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);


	void Rotate(float x, float y, float z);
	Matrix4x4 GetMatrix();

private:
	float  m_scale = 1.0f;
	Vector3f m_Pos = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f m_Rot = Vector3f(0.0f, 0.0f, 0.0f);
};