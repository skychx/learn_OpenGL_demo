#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // 实例化 GLFW 窗口
    glfwInit();
    // 告知 glfw OpenGL 版本为 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 创建窗口对象，这个窗口对象存放了所有和窗口相关的数据
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);
    // 注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 在 Clion 中，cpp 源文件经编译后生成可执行文件，
    // 放在 cmake-build-debug 目录下，也就是最终的执行目录，所以文件相对路径应该是 ../
    Shader ourShader("../shaders/1.3.shader.vs", "../shaders/1.3.shader.fs");

    // 2D 三角形
    float vertices[] = {
            // 位置              // 颜色
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    // 顶点缓冲对象
    // 它会在GPU内存（通常被称为显存）中储存大量顶点。
    // 使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。
    unsigned int VBO;
    unsigned int VAO; // 顶点数组对象，绑定后任何随后的顶点属性调用都会储存在这个 VAO 中
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); // 绑定VAO

    // 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把之前定义的顶点数据复制到缓冲的内存中

    // 告诉OpenGL该如何解析顶点数据
    // 第一个参数指定我们要配置的顶点属性 layout(location = 0)
    // 第二个参数指定顶点属性的大小 vec3
    // 第三个参数指定数据的类型 GL_FLOAT
    // 第四个参数我们是否希望数据被标准化 GL_FALSE
    // 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔 3 * sizeof(float)
    // 最后一个参数的类型是void*
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 以顶点属性位置值作为参数，启用顶点属性
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1); // 以顶点属性位置值作为参数，启用顶点属性

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO);

    // GL_LINE 用线绘制，GL_FILL 用面绘制
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // render loop
    // glfwWindowShouldClose 函数在我们每次循环的开始前检查一次GLFW是否被要求退出
    while(!glfwWindowShouldClose(window)) {
        // input
        // ----
        // 每次循环的时候监听键盘事件
        processInput(window);

        // render
        // -----
        // 当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数
        glClear(GL_COLOR_BUFFER_BIT); // 状态使用函数

        // 激活程序对象
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 使用当前激活的着色器，之前定义的顶点属性配置，和VBO的顶点数据（通过VAO间接绑定）来绘制图元

        // glfwSwapBuffers函数会交换颜色缓冲（知识点：双缓冲技术）
        glfwSwapBuffers(window);
        // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // 渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

//检查用户是否按下了返回键(Esc)
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度
    glViewport(0, 0, width, height);
}