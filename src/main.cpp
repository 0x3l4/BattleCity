#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Массив вершин.
// Вертексный шейдер будет запускаться столько раз, сколько у нас вершин.
// Фрагментный шейдер будет запускаться столько раз, сколько у нас видимых фрагментов (пикселей).
GLfloat points[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

// Массив цветов вершин
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};


// Вертексный шейдер.
// Пишется на языке GLSL (OpengGL Shader Language).
const char* vertex_shader =
"#version 460\n"                                    // (Макрос) Версия шейдера 4.6.0.
"layout(location = 0) in vec3 vertex_position;\n"   // Входной параметр - позиция вершины.
"layout(location = 1) in vec3 vertex_color;\n"      // Входной параметр - цвет вершины.
"out vec3 color;\n"                                 // Выходной параметр - цвет.
"void main() {\n"                                   // Объявление функции Main.
"   color = vertex_color;\n"                        // Присваиваем выходному параметру входной параметр.
"   gl_Position = vec4(vertex_position, 1.0)\n;"    // Присваиваем позицию из входного параметра (4й параметр - это перспектива).
"}";

const char* fragment_shader =
"#version 460\n"                                    // (Макрос) Версия шейдера 4.6.0.
"in vec3 color;"                                    // Входной параметр - цвет.
"out vec4 frag_color;"                              // Выходной параметр - интерполяция цвета.
"void main() {"                                     // Объявляем функцию Main.
"   frag_color = vec4(color, 1.0);"                 // Присваиваем выходному параметру текущий цвет.
"}";


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

int main(void)
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

    
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);   // Создаём индентификатор вертексного шейдера..
    glShaderSource(vertexShaderID, 1, &vertex_shader, nullptr); // Передаём вертексный шейдер по идентификатору.
    glCompileShader(vertexShaderID);                            // Компилируем код шейдера.

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);   // Создаём индентификатор фрагментого шейдера.
    glShaderSource(fragmentShaderID, 1, &fragment_shader, nullptr); // Передаёт фрагментный шейдер по идентификатору.
    glCompileShader(fragmentShaderID);                              // Компилируем фрагментный шейдер.

    GLuint shaderProgramID = glCreateProgram();          // Создаём программу для линковки шейдеров.
    glAttachShader(shaderProgramID, vertexShaderID);     // Линкуем индентификатор вертексного шейдера.
    glAttachShader(shaderProgramID, fragmentShaderID);   // Линкуем идентификатор фрагментного шейдера.
    glLinkProgram(shaderProgramID);                      // Соединяем.

    // После линковки программы шейдеры можно удалить.
    glDeleteShader(vertexShaderID);     // Удаляем вертексный шейдер.
    glDeleteShader(fragmentShaderID);   // Удаляем фрагментный шейдер.

    // Теперь шейдеры нужно передать нашей видеокарте.
    GLuint pointsVboID = 0;                                                 // Создаём идентификатор буффера вершин.
    glGenBuffers(1, &pointsVboID);                                          // Драйвер генерирует для него значение.
    glBindBuffer(GL_ARRAY_BUFFER, pointsVboID);                             // Делаем буффер текущим (как массив).
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);  // Передаём вершины в память видеокарты.

    GLuint colorsVboID = 0;                                                 // Создаём идентификатор буффера цвета.
    glGenBuffers(1, &colorsVboID);                                          // Драйвер генерирует для него значение.
    glBindBuffer(GL_ARRAY_BUFFER, colorsVboID);                             // Делаем буффер текущим (как массив).
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);  // Передаём цвета в память видеокарты.

    GLuint vaoID = 0;               // Создаём идентификатор хендлера над данными.
    glGenVertexArrays(1, &vaoID);   // Генерируем для него значение.
    glBindVertexArray(vaoID);       // Делаем текущим.

    // Должны связать pointsVbo и colorsVbo.
    glEnableVertexAttribArray(0);                                   // Включаем первый.
    glBindBuffer(GL_ARRAY_BUFFER, pointsVboID);                     // Делаем текущим вершины.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    // Для 0 - 3 элемента float, ненормированы, смещение 0.

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVboID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка треугольника.
        glUseProgram(shaderProgramID);      // Подключаем шейдеры.
        glBindVertexArray(vaoID);           // Какой вертекс отобразить.
        glDrawArrays(GL_TRIANGLES, 0, 3);   // Отрисовка треугольника.

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}