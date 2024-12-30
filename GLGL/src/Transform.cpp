#include "Transform.h"

void Transform::SetScale(float scale){
	m_scale = scale;
}
void Transform::SetRotation(float x, float y, float z) {
	m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;
}
void Transform::SetPosition(float x, float y, float z){
	m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;
}


void Transform::Rotate(float x, float y, float z){
	m_Rot.x += x; m_Rot.y += y; m_Rot.z += z;
}
Matrix4x4 Transform::GetMatrix(){
	Matrix4x4 scale;
	scale.InitScaleTransform(m_scale);

	Matrix4x4 rotation;
	rotation.InitRotationTransform(m_Rot.x, m_Rot.y, m_Rot.z);

	Matrix4x4 translation;
	translation.InitTranslationTransform(m_Pos.x, m_Pos.y, m_Pos.z); 

	//rotation.Print();

	return translation * rotation * scale;
}