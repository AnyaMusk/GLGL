#pragma once

#include<cmath>
#include<math.h>
#include<glm/vec3.hpp>
#include<iostream>

#define PI 3.14159265359
#define Deg2Rad(degree) (float)(degree * PI / 180.0f)
#define Rad2Deg(rad) (float)(rad * 180.0f / PI)

struct PerpectiveProjInfo {
    float fov;
    int* width;
    int* height;
    float zNear;
    float zFar;
};

struct Vector2f {
    float x;
    float y;

    Vector2f() {
        x = y = 0;
    }

    Vector2f(float _x, float _y) {
        x = _x;
        y = _y;
    }

    Vector2f(float t) {
        x = y = t;
    }

    inline Vector2f operator + (const Vector2f& v) {
        return Vector2f(this->x + v.x, this->y + v.y);
    }

    inline Vector2f operator * (const float& s) {
        return Vector2f(s * this->x, s * this->y);
    }

    Vector2f& operator+=(const Vector2f& r)
    {
        x += r.x;
        y += r.y;

        return *this;
    }

    Vector2f& operator-=(const Vector2f& r)
    {
        x -= r.x;
        y -= r.y;

        return *this;
    }

    Vector2f& operator*=(float f)
    {
        x *= f;
        y *= f;
        return *this;
    }

    bool operator==(const Vector2f& r)
    {
        return ((x == r.x) && (y == r.y));
    }

    bool operator!=(const Vector2f& r)
    {
        return !(*this == r);
    }

    float Length() {
        return sqrtf(this->x * this->x + this->y * this->y);
    }

    Vector2f& Normalise() {
        float length = Length();

        this->x /= length;
        this->y /= length;

        return *this;
    }
    void Print() {
        printf("%f %f\n", this->x, this->y);
    }
};


struct  Vector3f
{
	float x;
	float y;
	float z;

	Vector3f() {
		x = y = z = 0;
	}

	Vector3f(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f(float t) {
		x = y = z = t;
	}

    inline Vector3f operator + (const Vector3f& v) {
        return Vector3f(this->x + v.x, this->y + v.y, this->z + v.z);  
    }

    inline Vector3f operator * (const float& s) {
        return Vector3f (s * this->x, s * this->y, s * this->z);
    }

    Vector3f& operator+=(const Vector3f& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

    Vector3f& operator-=(const Vector3f& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }
    
    Vector3f& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    bool operator==(const Vector3f& r)
    {
        return ((x == r.x) && (y == r.y) && (z == r.z));
    }

    bool operator!=(const Vector3f& r)
    {
        return !(*this == r);
    }
    
    float Length() {
        return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    Vector3f& Normalise() {
        float length = Length();

        if (length == 0)
            return *this;

        this->x /= length;
        this->y /= length;
        this->z /= length;

        return *this;
    }

    Vector3f Cross(Vector3f& v) {
        float _x = y * v.z - z * v.y;
        float _y = z * v.x - x * v.z;
        float _z = x * v.y - y * v.x;

        return Vector3f(_x, _y, _z);
    }

    void Rotate(float angle, const Vector3f& axis);

    void Print() {
        printf("%f %f % f \n", this->x, this->y, this->z);
    }
};

class Matrix4x4
{
public:
    float m[4][4];

    Matrix4x4() {}

    Matrix4x4(float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33)
    {
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
        m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }

    Matrix4x4 Transpose() const
    {
        Matrix4x4 n;

        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                n.m[i][j] = m[j][i];
            }
        }

        return n;
    }


    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4x4 operator *(const Matrix4x4& Right) const
    {
        Matrix4x4 Ret;

        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                    m[i][1] * Right.m[1][j] +
                    m[i][2] * Right.m[2][j] +
                    m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }
    void Print() const
    {
        for (int i = 0; i < 4; i++) {
            printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
        }
        std::cout << std::endl;
    }

    void InitScaleTransform(float scaleX, float scaleY, float scaleZ);
    void InitScaleTransform(const Vector3f& scaleVector);
    void InitScaleTransform(float scale);
    
    void InitRotationTransform(float rotateX, float rotateY, float rotateZ);
    void InitRotationX(float rotateX);
    void InitRotationY(float rotateY);
    void InitRotationZ(float rotateZ);

    void InitTranslationTransform(float x, float y, float z);

    void InitCameraTransform(Vector3f m_target, Vector3f m_up);
    void InitCameraTransform(Vector3f m_pos, Vector3f m_target, Vector3f m_up);

    void InitPerspectiveProjectionTransform(const PerpectiveProjInfo& p);
};


struct  Quaternion
{
public:
    float w, x, y, z;
    Quaternion(float angle, const Vector3f& v);
    Quaternion(float w, float x, float y, float z);

    void Normalize();
    Quaternion Conjugate() const;
    Vector3f ToDegree();
    void Print() {
        printf("%f %f %f %f \n ", w, x, y, z);
    }
};

Quaternion operator * (const Quaternion& l, const Quaternion& r);
Quaternion operator * (const Quaternion& q, const Vector3f& v);
