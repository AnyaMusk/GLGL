#include "Camera.h"
#include<GLFW/glfw3.h>


const int MARGIN = 40;
const float EDGE_STEP = 1.0f;

Camera::Camera(int* windowWidth, int* windowHeight, bool* isInScreen) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    m_pos = Vector3f(0.0f, 0.0f, 0.0f);
    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_up = Vector3f(0.0f, 1.0f, 0.0f);  

    cursorInside = isInScreen;

    Init();
}

Camera::Camera(int* windowWidth, int* windowHeight, const Vector3f pos, const Vector3f target, const Vector3f up, bool* isInScreen) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    
    m_pos = pos;
	m_target = target;
    m_target.Normalise();
    //m_target.Print();
	m_up = up;
    m_up.Normalise();

    cursorInside = isInScreen;

    Init();
}

void Camera::Init() {
    Vector3f hTarget(m_target.x, 0.0f, m_target.z);
    hTarget.Normalise();

    // angle from y axis(global)
    m_horizontalAngle = -Rad2Deg(atan2(hTarget.z, hTarget.x));
    // angle is 90 when looking down, -90 when camera looking up so from, global x axis
    m_verticalAngle = -Rad2Deg(asin(m_target.y));

    //std::cout << m_horizontalAngle << "  " << m_verticalAngle << std::endl;


    m_onUpperEdge = false;
    m_onLowerEdge = false;
    m_onLeftEdge = false;
    m_onRightEdge = false;

    m_mousePos.x = *m_windowWidth * 0.5f;
    m_mousePos.y = *m_windowHeight * 0.5f;
}


void Camera::OnMouse(double x, double y) {

    Vector3f hTarget(m_target.x, 0.0f, m_target.z);
    hTarget.Normalise();

    // angle from y axis(global)
    m_horizontalAngle = -Rad2Deg(atan2(hTarget.z, hTarget.x));
    // angle is 90 when looking down, -90 when camera looking up so from, global x axis
    m_verticalAngle = -Rad2Deg(asin(m_target.y));

   // printf("horizontal %f : vertical %f \n", m_horizontalAngle, m_verticalAngle);


    double deltaX = x - m_mousePos.x;
    double deltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_horizontalAngle += deltaX * 0.05f;
    m_verticalAngle += deltaY * 0.05f;

    if (deltaX == 0) {
        if (x <= MARGIN) {
            m_onLeftEdge = true;
        }
        else if(x >= *m_windowWidth - MARGIN) {
            m_onRightEdge = true;
        }
        else {
            m_onLeftEdge = false;
            m_onRightEdge = false;
        }
    }

    if (deltaY == 0) {
        if (y <= MARGIN) {
            m_onUpperEdge = true;
        }
        else if (y >= (*m_windowHeight - MARGIN)) {
            m_onLowerEdge = true;
        }
    }
    else {
        m_onUpperEdge = false;
        m_onLowerEdge = false;
    }

    if (*cursorInside) {
        Update();
    }
}

void Camera::OnRender() {
    bool ShouldUpdate = false;

    if (m_onLeftEdge) {
        m_horizontalAngle -= EDGE_STEP;
        ShouldUpdate = true;
    }
    else if (m_onRightEdge) {
        m_horizontalAngle += EDGE_STEP;
        ShouldUpdate = true;
    }

    if (m_onUpperEdge) {
        if (m_verticalAngle > -90.0f) {
            m_verticalAngle -= EDGE_STEP;
            ShouldUpdate = true;
        }
    }
    else if (m_onLowerEdge) {
        if (m_verticalAngle < 90.0f) {
            m_verticalAngle += EDGE_STEP;
            ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}

void Camera::Update() {
    
    Vector3f upAxis(0.0f, 1.0f, 0.0f);

    // this is the target axis of cam
    Vector3f view(1.0f, 0.0f, 0.0f);
    // need to rotate this according to horizontal angle
    view.Rotate(m_horizontalAngle, upAxis);
    view.Normalise();
   
    Vector3f u = upAxis.Cross(view);
    u.Normalise();

    view.Rotate(m_verticalAngle, u);
    m_target = view;
    m_target.Normalise();

    m_up = m_target.Cross(u);
    m_up.Normalise();


}

void Camera::SetPosition(float x, float y, float z) {
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void Camera::OnKeyboard(int key, float deltaTime) {
    switch (key) {
    case GLFW_KEY_W:
        m_pos += (m_target * m_speed * deltaTime);
        break;

    case GLFW_KEY_S:
        m_pos -= (m_target * m_speed * deltaTime);
        break;

    case GLFW_KEY_Q:
        m_pos -= (m_up * m_speed * deltaTime);
        break;

    case GLFW_KEY_E:
        m_pos += (m_up * m_speed * deltaTime);
        break;

    case GLFW_KEY_A: {
        Vector3f left = m_target.Cross(m_up);
        left.Normalise();
        m_pos += (left * m_speed * deltaTime) ;
        break;
    }

    case GLFW_KEY_D: {
        Vector3f right = m_up.Cross(m_target);
        right.Normalise();
        m_pos += (right * m_speed * deltaTime);
        break;
    }

    case GLFW_KEY_PAGE_UP:
        m_pos.y += m_speed;
        break;

    case GLFW_KEY_PAGE_DOWN:
        m_pos.y -= m_speed;
        break;

    case GLFW_KEY_KP_ADD: // Handle '+' on numeric keypad
    case GLFW_KEY_EQUAL:  // Handle '=' (Shift required for '+')
        m_speed += 0.1f;
        printf("Speed changed to %f\n", m_speed);
        break;

    case GLFW_KEY_KP_SUBTRACT: // Handle '-' on numeric keypad
    case GLFW_KEY_MINUS:       // Handle '-' on regular keyboard
        m_speed -= 0.1f;
        if (m_speed < 0.1f) {
            m_speed = 0.1f;
        }
        printf("Speed changed to %f\n", m_speed);
        break;

    default:
        break;
    }
}

Matrix4x4 Camera::GetMatrix(){
	Matrix4x4 cameraTransformation;
	cameraTransformation.InitCameraTransform(m_pos, m_target, m_up);

	return cameraTransformation;
}

