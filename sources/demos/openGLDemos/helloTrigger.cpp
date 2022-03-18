#include "helloTrigger.h"
#include "OpenGLInclude.h"
#include "common/Program.h"
#include "Defines.h"
#include <iostream>

using namespace OpenGLDemos;

const std::string c_glsl_vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 pointColor;
void main()
{
    pointColor = aColor;
    gl_Position = vec4(aPos, 1.0f);
}
)";

const std::string c_glsl_fs = R"(
#version 330 core
in vec3 pointColor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(pointColor, 1.0f);
}
)";

HelloTrigger::~HelloTrigger()
{
	delete m_program;
}

void HelloTrigger::execute()
{
	HelloTrigger *helloTrigger = new HelloTrigger;
	if (helloTrigger->init())
	{
		helloTrigger->run();
	}
	glfwTerminate();
	return;
}

bool HelloTrigger::init()
{
	//初始化glfw
	glfwInit();
	//设置openGL的版本为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置openGL核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口
	m_window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	//设置窗口大小改变的回调函数，用以改变viewport
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
		std::cout << "glfwSetFreameBufferSizeCallBack : " << width << "  " << height;
		std::flush(std::cout);
		glViewport(0, 0, width, height);
	});
	//设置openGL上下文
	glfwMakeContextCurrent(m_window);
	//初始化glad，在调用openGL的相关函数之前调用，否则会使用openGL相关函数时，会报空指针异常
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "load glad failed : " << std::endl;
		return false;
	}
	float points[] = {
		-0.5, -0.5, 0, 0.1, 0.1, 1.0,
		0.5, -0.5, 0, 1.0, 0.1, 0.1,
		0.5, 0.5, 0, 0.5, 0.5, 0.2,
		-0.5, 0.5, 0, 0.1, 0.5, 0.5
	};

	unsigned int trangleIndexes[6] = {
		0, 1, 2,
		2, 3, 0
	};

	//编译vsShader
	m_program = Common::Program::create(c_glsl_vs, c_glsl_fs);
	if (m_program == nullptr)
	{
		OD_Assert(false);
		return false;
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trangleIndexes), trangleIndexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}

void HelloTrigger::run()
{
	//如果没有关闭，做循环
	while (!glfwWindowShouldClose(m_window))
	{
		//循环开始先处理事件
		processEvent();
		glfwMakeContextCurrent(m_window);
		//设置清除颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清除颜色
		glClear(GL_COLOR_BUFFER_BIT);
		m_program->use();
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//交换缓冲区，否则绘制时，会有从上到下，从左到右的线条打印
		glfwSwapBuffers(m_window);
		//处理鼠标键盘事件。此代码缺失，会导致鼠标键盘在创建的openGL窗口上无响应
		glfwPollEvents();
	}

	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void HelloTrigger::processEvent()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == true)
	{
		glfwSetWindowShouldClose(m_window, true);
	}
}
