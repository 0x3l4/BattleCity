#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

// ������ ������.
// ���������� ������ ����� ����������� ������� ���, ������� � ��� ������.
// ����������� ������ ����� ����������� ������� ���, ������� � ��� ������� ���������� (��������).
GLfloat points[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

// ������ ������ ������
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char* argv[])
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Initialization glfw failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Creation a glfw window failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 0, 1);

    ResourceManager resourceManager(argv[0]);
    auto defaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");

    if (defaultShaderProgram == nullptr)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return -1;
    }

    // ������ ������� ����� �������� ����� ����������.
    GLuint pointsVboID = 0;                                                 // ������ ������������� ������� ������.
    glGenBuffers(1, &pointsVboID);                                          // ������� ���������� ��� ���� ��������.
    glBindBuffer(GL_ARRAY_BUFFER, pointsVboID);                             // ������ ������ ������� (��� ������).
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);  // ������� ������� � ������ ����������.

    GLuint colorsVboID = 0;                                                 // ������ ������������� ������� �����.
    glGenBuffers(1, &colorsVboID);                                          // ������� ���������� ��� ���� ��������.
    glBindBuffer(GL_ARRAY_BUFFER, colorsVboID);                             // ������ ������ ������� (��� ������).
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);  // ������� ����� � ������ ����������.

    GLuint vaoID = 0;               // ������ ������������� �������� ��� �������.
    glGenVertexArrays(1, &vaoID);   // ���������� ��� ���� ��������.
    glBindVertexArray(vaoID);       // ������ �������.

    // ������ ������� pointsVbo � colorsVbo.
    glEnableVertexAttribArray(0);                                   // �������� ������.
    glBindBuffer(GL_ARRAY_BUFFER, pointsVboID);                     // ������ ������� �������.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    // ��� 0 - 3 �������� float, �������������, �������� 0.

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVboID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // ��������� ������������. 
        defaultShaderProgram->use();
        glBindVertexArray(vaoID);           // ����� ������� ����������.
        glDrawArrays(GL_TRIANGLES, 0, 3);   // ��������� ������������.

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    defaultShaderProgram->~ShaderProgram();

    glfwTerminate();
    return 0;
}