#include "Shader.h"
#include "MathUtil.h"
#include"Transform.h"
#include "Camera.h"
#include"Texture.h"


unsigned int VBO, VAO, IBO;
float ar;


unsigned int matrixLocation;
unsigned int samplerLocation;
Texture* texture;


const unsigned int steps = 5;
float angle = (2.0f * PI) / steps;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void key_call_back(GLFWwindow* window, int key, int scan, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double x, double y);
void cursor_enter_callback(GLFWwindow* window, int entered);

float deltaTime;
float lastFrame;

void CalculateDelta() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int width = 0, height = 0;

const char* vertexfilename = "assets/vs.glsl";
const char* fragmentfilename = "assets/fs.glsl";

Transform cubeTransform;

Vector3f cameraPos(0.0f, 0.0f, -1.0f);
Vector3f cameraTarget(0.0f, 0.0f, 1.0f);
Vector3f cameraUp(0.0f, 1.0f, 0.0f);

bool isCursorInside;

Camera gameCamera(&width, &height, cameraPos, cameraTarget, cameraUp, &isCursorInside);


// allocate a program handle
   // allocate a shader handle
   // load code file from glsl file
   // load shader code to shader handle
   // compile the shader
   // link program
   // enable program


float FOV = 90.0f;
float zNear = 1.0f;
float zFar = 10.0f;

PerpectiveProjInfo p = { FOV, &width, &height, zNear, zFar };


struct Vertex {
public:
    Vector3f position;
    Vector2f texture;

    Vertex() {}

    Vertex(const Vector3f& pos, const Vector2f& tex) {
        position = pos;
        texture = tex;
    }
};


static void CreateCube() {
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    Vector2f t00 = Vector2f(0.0f, 0.0f);
    Vector2f t10 = Vector2f(1.0f, 0.0f);
    Vector2f t01 = Vector2f(0.0f, 1.0f);
    Vector2f t11 = Vector2f(1.0f, 1.0f);

    Vertex vertices[8] = {
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), t00),
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), t01),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), t10),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), t11),
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), t00),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), t10),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), t01),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), t11)
    };

    unsigned int indices[]{
        0, 1, 2,
        1, 3, 4, 
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1, 
        1, 5, 3,
        5, 0, 6, 
        7, 4, 3, 
        2, 1, 4, 
        0, 2, 7
    };
      
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    texture->Bind(GL_TEXTURE0);
    glUniform1i(samplerLocation, 0);
}

static void RotationExample() {
    static float angleInRad = 0.0f;
    static float delta = 0.01f;

    angleInRad += delta;
    if (angleInRad < -PI / 2.0f || angleInRad > PI / 2.0f) {
        delta *= -1.0f;
    }

    // rotating in z
    Matrix4x4 rot(cosf(angleInRad), -sinf(angleInRad), 0.0f, 0.0f,
                sinf(angleInRad), cosf(angleInRad), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(matrixLocation, 1, false , &rot.m[0][0]);
}

static void ScalingExample() {
    static float scale = 1.0f;
    static float delta = 0.01f;

    scale += delta;
    if (scale <= 0.5f || scale > 1.5f) {
        delta *= -1.0f;
    }

    Matrix4x4 s(scale, 0.0f, 0.0f, 0.0f,
                0.0f, scale, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, &s.m[0][0]);
}

static void CombiningTransformation1() {
    static float scale = 1.5f;

    Matrix4x4 s
        (scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    static float loc = 0.0f;
    static float delta = 0.01f;

    loc += delta;
    if (loc <= -1.0f || loc >= 1.0f) {
        delta *= -1.0f;
    }

    Matrix4x4 t
        (1.0f, 0.0f, 0.0f, loc,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    // transformation is scaled
    //Matrix4x4 finalMatrix = s * t;
    // first scaled then transformed
    Matrix4x4 finalMatrix = t * s;


    glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, &finalMatrix.m[0][0]);
}

static void CombiningTransformation2() {
    static float scale = 0.5f;

    Matrix4x4 s
        (scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    static float angleInRad = 0.0f;
    static float delta = 0.01f;

    angleInRad += delta;

    // rotating in z
    Matrix4x4 rot(cosf(angleInRad), -sinf(angleInRad), 0.0f, 0.0f,
        sinf(angleInRad), cosf(angleInRad), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    static float loc = 0.5f;

    Matrix4x4 t
        (1.0f, 0.0f, 0.0f, loc,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    //Matrix4x4 finalMatrix = t * rot * s;
    // here basically the translation matrix is being rotated
    Matrix4x4 finalMatrix = rot * t * s;    

    glUniformMatrix4fv(matrixLocation, 1, true, &finalMatrix.m[0][0]);
}

static void CubeTransformation() {
    static float angle = 0.01f;
    //float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;

    angle += 0.01f;

    Matrix4x4 rot(cosf(angle), 0.0f, sinf(angle), 0.0f,
                  0.0f,        1.0f, 0.0f,        0.0f,
                 -sinf(angle), 0.0f, cosf(angle), 0.0f,
                  0.0f,        0.0f, 0.0f,        1.0f);

    Matrix4x4 trans(1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 3.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4x4 worldTrans = trans * rot;

    Vector3f cameraPos(1.0f, -1.0f, -1.0f);
    Vector3f U(1.0f, 0.0f, 0.0f);
    Vector3f V(0.0f, 1.0f, 0.0f);
    Vector3f N(0.0f, 0.0f, 1.0f);

    // this is inversed
    Matrix4x4 Camera(U.x, U.y, U.z, -cameraPos.x,
                    V.x, V.y, V.z, -cameraPos.y,
                    N.x, N.y, N.z, -cameraPos.z, 
                    0.0f, 0.0f, 0.0f, 1.0f); 

    float FOV = 60.0f;

    float tanOfHalfFov = tanf(Deg2Rad(FOV) / 2.0f);
    float f = 1.0f / tanOfHalfFov;

    float nearZ = 2.0f;
    float farZ = 10.0f;
    float zRange = nearZ - farZ;
    
    float A = (-nearZ - farZ) / zRange;
    float B = (2 * nearZ * farZ) / zRange;

    Matrix4x4 projection(f / ar, 0.0f, 0.0f, 0.0f,
                        0.0f, f, 0.0f, 0.0f,
                        0.0f, 0.0f, A, B,
                        0.0f, 0.0f, 1.0f, 0.0f);


    // mvp
    Matrix4x4 mat = projection * Camera * worldTrans;
    glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, &mat.m[0][0]);
}

static void CubeTrans2() {
    float yRotAngle = 0.3f;

    cubeTransform.SetPosition(0.0f, 0.0f, 2.0f);
    cubeTransform.Rotate(0.0f, yRotAngle, 0.0f);
    Matrix4x4 worldMatrix = cubeTransform.GetMatrix();

    Matrix4x4 viewMatrix = gameCamera.GetMatrix();

    Matrix4x4 projectionMatrix;
    projectionMatrix.InitPerspectiveProjectionTransform(p);

    Matrix4x4 wvp = projectionMatrix * viewMatrix * worldMatrix;
    glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, &wvp.m[0][0]);
}




int main()
{   
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //int count;
    //GLFWmonitor** monitors = glfwGetMonitors(&count);
    //
    //// secondary
    //const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);
    //GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "HeyWindow", monitors[0],  NULL);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HeyWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_call_back);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    
    glfwSetCursorPos(window, SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f);

    glfwSetCursorEnterCallback(window, cursor_enter_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader s1(vertexfilename, fragmentfilename);
    
    //CreateVertexBuffer();
    //CreateCircle();
    //CreateTriangle();
    //CreateLotOfTriangles();
   

   
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(0);

    // getting uniforms
    matrixLocation = glGetUniformLocation(s1.id, "matrix");
    if (matrixLocation == -1) {
        std::cout << "Failed to retrieve mat4 uniforms" << std::endl;
    }

    samplerLocation = glGetUniformLocation(s1.id, "sampler");
    if (samplerLocation == -1) {
        std::cout << "Failed to retrieve mat4 uniforms" << std::endl;
    }

    texture = new Texture(GL_TEXTURE_2D, "content/1.jpg");

    if (!texture->Load()) {
        std::cout << "Could run texture" << std::endl;
        return 1;
    }

    CreateCube();

    while (!glfwWindowShouldClose(window))
    {
        CalculateDelta();
        gameCamera.OnRender();
        ProcessInput(window);
        glfwGetWindowSize(window, &width, &height);
        ar = (float)width / (float)height;
      
        // render loop
     
        //glClearColor(1.0f, 0.5f, 0.3f, 1.0f);

        // for window
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(VAO);
        s1.UseProgram();

        //RotationExample();
        //ScalingExample();
        //CombiningTransformation1();
        //CombiningTransformation2();
        //CubeTransformation();
        CubeTrans2();

        // 6 is number of indices
        //glDrawElements(GL_TRIANGLES, steps * 3, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, nullptr);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    s1.DeleteProgram();
    glfwTerminate();
    return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_call_back(GLFWwindow* window, int key, int scan, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        gameCamera.OnKeyboard(key, deltaTime);
    }
}

void cursor_pos_callback(GLFWwindow* window, double x, double y) {
    gameCamera.OnMouse(x, y);
}

void cursor_enter_callback(GLFWwindow* window, int entered) {
    isCursorInside = entered;
}

