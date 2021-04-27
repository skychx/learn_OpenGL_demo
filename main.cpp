#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL 核心模式
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS 配置

    // 创建窗口对象，这个窗口对象存放了所有和窗口相关的数据
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 创建完窗口我们就可以通知 GLFW 将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);
    // 注册这个函数，告诉 GLFW 我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD 是用来管理 OpenGL 的函数指针的，所以在调用任何 OpenGL 的函数之前我们需要初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 开启深度测试（z-buffer）
    glEnable(GL_DEPTH_TEST);

    // 在 Clion 中，cpp 源文件经编译后生成可执行文件，
    // 放在 cmake-build-debug 目录下，也就是最终的执行目录，所以文件相对路径应该是 ../
    Shader ourShader("../shaders/1.5.transShader.vert", "../shaders/1.5.transShader.frag");

    // 顶点输入
    // 3 - 0
    // | \ |
    // 2 - 1
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // 顶点缓冲对象 Vertex Buffer Objects（VBO）
    // 它会在 GPU 内存（通常被称为显存）中储存大量顶点。
    // 使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。
    unsigned int VBO;
    unsigned int VAO; // 顶点数组对象，绑定后任何随后的顶点属性调用都会储存在这个 VAO 中
    glGenBuffers(1, &VBO); // 利用 glGenBuffers 和一个缓冲 ID 生成一个 VBO 对象
    glGenVertexArrays(1, &VAO);

    // 一个顶点数组对象会储存以下这些内容：
    // - glEnableVertexAttribArray 和 glDisableVertexAttribArray 的调用
    // - 通过 glVertexAttribPointer 设置的顶点属性配置
    // - 通过 glVertexAttribPointer 调用与顶点属性关联的顶点缓冲对象
    glBindVertexArray(VAO); // 绑定VAO

    // 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把新创建的缓冲绑定到 GL_ARRAY_BUFFER 目标上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把之前定义的顶点数据复制到缓冲的内存中

    // ⬇️ 链接顶点属性

    // 告诉OpenGL该如何解析顶点数据
    // 第一个参数指定我们要配置的顶点属性：在着色器程序里定义的 layout(location = 0)
    // 第二个参数指定顶点属性的大小：vec3
    // 第三个参数指定数据的类型：GL_FLOAT
    // 第四个参数我们是否希望数据被标准化：GL_FALSE
    // 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔：3 + 3 + 2 = 8
    // 最后一个参数的类型是void*
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0); // 以顶点属性位置值作为参数，启用顶点属性
    // 纹理属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int texture1, texture2;
    stbi_set_flip_vertically_on_load(true); // 图片 y 轴 0.0 坐标一般在顶部，这个 API 是用来翻转 y 轴的
    glGenTextures(1, &texture1); // 输入生成纹理的数量: 1，然后把它们储存在第二个参数的 unsigned int 数组中
    glBindTexture(GL_TEXTURE_2D, texture1); // 绑定纹理
    // 为当前绑定的纹理对象设置环绕、过滤方式
    // GL_TEXTURE_WRAP_S/T: 纹理的坐标轴
    // GL_REPEAT: 重复纹理图像
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // GL_TEXTURE_MIN_FILTER/MAG_FILTER: 放大、缩小操作的时候设置纹理过滤的选项
    // GL_LINEAR: 线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../resources/textures/container.jpeg", &width, &height, &nrChannels, 0);
    if (data) {
        // 生成纹理：纹理目标，mipmap级别，格式，最终纹理的长宽，0（历史遗留），源图的格式和数据类型，真正的图像数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // 生成 Mipmap（多级渐远纹理）
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("../resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    ourShader.use(); // 不要忘记在设置 uniform 变量之前激活着色器程序！
    // 通过使用 glUniform1i 设置每个采样器的方式，告诉 OpenGL 每个着色器采样器属于哪个纹理单元
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
    ourShader.setInt("texture2", 1); // 或者使用着色器类设置

    // GL_LINE 用线绘制，GL_FILL 用面绘制
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // render loop
    // glfwWindowShouldClose 函数在我们每次循环的开始前检查一次 GLFW 是否被要求退出
    while(!glfwWindowShouldClose(window)) {
        // input
        // ----
        // 每次循环的时候监听键盘事件
        processInput(window);

        // render
        // -----
        // 当调用 glClear 函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为 glClearColor 里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲和深度缓冲

        // bind Texture
        // OpenGL 至少保证有 16 个纹理单元供你使用，也就是说你可以激活从 GL_TEXTURE0 到 GL_TEXTURE15
        glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1); // 绑定这个纹理到当前激活的纹理单元
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // 激活程序对象
        ourShader.use();

        // 变换
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        // 绕 x 轴旋转 55 度
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        // 反方向移动
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // 投影矩阵
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 使用当前激活的着色器，之前定义的顶点属性配置，和VBO的顶点数据（通过VAO间接绑定）来绘制图元
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // glfwSwapBuffers 函数会交换颜色缓冲（知识点：双缓冲技术）
        glfwSwapBuffers(window);
        // glfwPollEvents 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）
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