#pragma once
#include "MathUtil.h"

class Camera {
public:
	Camera(int* windowHeight, int* windowWidth, bool* isInScreen);
	Camera(int* windowHeight, int* windowWidth, const Vector3f pos, const Vector3f target, const Vector3f up, bool* isInScreen);
	void SetPosition(float x, float y, float z);
	void OnKeyboard(int key, float deltaTime);
	void OnMouse(double x, double y);
	void OnRender(); 
	Matrix4x4 GetMatrix();
private:

	Vector3f m_pos;
	// n in uvn ,target is where to look at
	Vector3f m_target;
	Vector3f m_up;
	float m_speed = 25.0f;

	int* m_windowWidth;
	int* m_windowHeight;

	// rotate around global y axis
	double m_horizontalAngle;
	// rotate around local u(x) axis.
	double m_verticalAngle;

	bool m_onUpperEdge;
	bool m_onLowerEdge;
	bool m_onLeftEdge;
	bool m_onRightEdge;

	bool* cursorInside;

	Vector2f m_mousePos;

	// to init horizontal and vertical values based on target and up vector
	void Init();
	void Update();
};