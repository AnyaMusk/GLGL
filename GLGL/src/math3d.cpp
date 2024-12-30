#include"MathUtil.h"

void Vector3f::Rotate(float angle, const Vector3f& axis) {
	Quaternion rot(angle, axis);
	Quaternion conjugate = rot.Conjugate();

	Quaternion result = rot * (*this) * conjugate;
	
	this->x = result.x;
	this->y = result.y;
	this->z = result.z;
}


void Matrix4x4::InitScaleTransform(float scaleX, float scaleY, float scaleZ) {
	m[0][0] = scaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
	m[1][0] = 0.0f;   m[1][1] = scaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
	m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = scaleZ; m[2][3] = 0.0f;
	m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}
void Matrix4x4::InitScaleTransform(const Vector3f& scaleVector){
	InitScaleTransform(scaleVector.x, scaleVector.y, scaleVector.z);
}

void Matrix4x4::InitScaleTransform(float scale) {
	InitScaleTransform(scale, scale, scale);
}

void Matrix4x4::InitRotationX(float x) {
	m[0][0] = 1.0f; m[0][1] = 0.0f;  m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = cosf(x);  m[1][2] = -sinf(x);  m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = sinf(x); m[2][2] =  cosf(x); m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f;  m[3][2] = 0.0f; m[3][3] = 1.0f;
}
void Matrix4x4::InitRotationY(float y) {
	m[0][0] = cosf(y); m[0][1] = 0.0f; m[0][2] = sinf(y); m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = -sinf(y); m[2][1] = 0.0f; m[2][2] = cosf(y); m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}
void Matrix4x4::InitRotationZ(float z) {
	m[0][0] = cosf(z);  m[0][1] = -sinf(z);  m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = sinf(z);  m[1][1] = cosf(z); m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f;  m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f;  m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4x4::InitRotationTransform(float rotateX, float rotateY, float rotateZ){
	Matrix4x4 rx, ry, rz;

	float x = Deg2Rad(rotateX);
	float y = Deg2Rad(rotateY);
	float z = Deg2Rad(rotateZ);

	rx.InitRotationX(x);
	ry.InitRotationY(y);
	rz.InitRotationZ(z);

	*this = rz * ry * rx;
}
void Matrix4x4::InitTranslationTransform(float x, float y, float z){
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}
void Matrix4x4::InitCameraTransform(Vector3f m_target, Vector3f m_up) {
	// left hand coordinate system
	Vector3f N = m_target;
	N.Normalise();

	// right
	Vector3f U = m_up.Cross(N);
	U.Normalise();

	// y
	Vector3f V = N.Cross(U);
	
	m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
	m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
	m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
	m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
}

void Matrix4x4::InitCameraTransform(Vector3f m_pos, Vector3f m_target, Vector3f m_up) {
	// look at
	Matrix4x4 cameraTranslationTrans;
	cameraTranslationTrans.InitTranslationTransform(-m_pos.x, -m_pos.y, -m_pos.z);
	
	Matrix4x4 cameraRotateTrans;
	cameraRotateTrans.InitCameraTransform(m_target, m_up);

	*this = cameraRotateTrans * cameraTranslationTrans;
}

void Matrix4x4::InitPerspectiveProjectionTransform(const PerpectiveProjInfo& p) {
	const float ar = (float)*p.width / (float)*p.height;
	const float zRange = p.zNear - p.zFar;
	const float tanfovHalf = tanf(Deg2Rad(p.fov / 2));

	m[0][0] = 1 / (tanfovHalf * ar); m[0][1] = 0.0f;                     m[0][2] = 0.0f;                         m[0][3] = 0.0;
	m[1][0] = 0.0f;                  m[1][1] = 1.0f / (tanfovHalf);      m[1][2] = 0.0f;                         m[1][3] = 0.0;
	m[2][0] = 0.0f;                  m[2][1] = 0.0f;                     m[2][2] = (-p.zNear - p.zFar) / zRange; m[2][3] = 2.0f * p.zFar * p.zNear / zRange;
	m[3][0] = 0.0f;                  m[3][1] = 0.0f;                     m[3][2] = 1.0f;                         m[3][3] = 0.0;
}

Quaternion::Quaternion(float angle, const Vector3f& v){
	float halfAngle = Deg2Rad(angle * 0.5f);

	float sinH = sinf(halfAngle);
	float cosH = cosf(halfAngle);

	w = cosH;
	x = v.x * sinH;
	y = v.y * sinH;
	z = v.z * sinH;
}
Quaternion::Quaternion(float _w, float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Quaternion::Normalize(){
	float l = sqrt(w * w + x * x + y * y + z * z);
	x /= l;
	y /= l;
	z /= l;
	w /= l;	
}
Quaternion Quaternion::Conjugate() const{
	// for normalised quaternions inverse is same as conjugate
	Quaternion ret(w, -x, -y, -z);
	return ret;
}
Vector3f Quaternion::ToDegree(){
	return Vector3f();
}
Quaternion operator * (const Quaternion& l, const Quaternion& r) {
	float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	Quaternion ret(w, x, y, z);

	return ret;
}
Quaternion operator * (const Quaternion& q, const Vector3f& v) {
	// quat is multiplied left
	float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	Quaternion ret(w, x, y, z);

	return ret;
}

