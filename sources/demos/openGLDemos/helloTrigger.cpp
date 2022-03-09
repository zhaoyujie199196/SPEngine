#include "helloTrigger.h"
#include "OpenGLInclude.h"
#include "common/Program.h"
#include "Defines.h"
#include <iostream>

using namespace OpenGLDemos;

const std::string c_glsl_vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos, 1.0f);
}
)";

const std::string c_glsl_fs = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.0f, 1.0f, 0.5f, 1.0f);
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
	//��ʼ��glfw
	glfwInit();
	//����openGL�İ汾Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//����openGL����ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//��������
	m_window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	//���ô��ڴ�С�ı�Ļص����������Ըı�viewport
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
		std::cout << "glfwSetFreameBufferSizeCallBack : " << width << "  " << height;
		std::flush(std::cout);
		glViewport(0, 0, width, height);
	});
	//����openGL������
	glfwMakeContextCurrent(m_window);
	//��ʼ��glad���ڵ���openGL����غ���֮ǰ���ã������ʹ��openGL��غ���ʱ���ᱨ��ָ���쳣
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "load glad failed : " << std::endl;
		return false;
	}
	float points[] = {
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0,
		-0.5, 0.5, 0
	};

	unsigned int trangleIndexes[6] = {
		0, 1, 2,
		2, 3, 0
	};

	//����vsShader
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}

void HelloTrigger::run()
{
	//���û�йرգ���ѭ��
	while (!glfwWindowShouldClose(m_window))
	{
		//ѭ����ʼ�ȴ����¼�
		processEvent();
		glfwMakeContextCurrent(m_window);
		//���������ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//�����ɫ
		glClear(GL_COLOR_BUFFER_BIT);
		m_program->apply();
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//�������������������ʱ�����д��ϵ��£������ҵ�������ӡ
		glfwSwapBuffers(m_window);
		//�����������¼����˴���ȱʧ���ᵼ���������ڴ�����openGL����������Ӧ
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
