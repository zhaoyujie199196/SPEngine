#include "textureDemo.h"
#include "OpenGLInclude.h"
#include "common/Program.h"
#include "utils/FileUtil.h"
#include "core/Application.h"
#include "common/Texture2D.h"

using namespace OpenGLDemos;

static const std::string c_glsl_vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
void main()
{
    TexCoord = aTexCoord;
    gl_Position = vec4(aPos, 1.0f);
}
)";
static const std::string c_glsl_fs = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D wallTexture;
void main()
{
    FragColor = texture(wallTexture, TexCoord);
}
)";

static const std::string c_pictureName = "wall.jpg";

void textureDemo::execute()
{
	textureDemo demo;
	if (!demo.init())
	{
		return;
	}
	demo.run();
}

bool textureDemo::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(800, 600, "textureDemo", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}
	
	//������λ�� ��������λ��
	float points[] = {
		-0.5, -0.5, 0,  0.0, 0.0,
		 0.5, -0.5, 0,  1.0, 0.0,
		 0.5,  0.5, 0,  1.0, 1.0,
		-0.5,  0.5, 0,  0.0, 1.0,
	};

	unsigned int indexes[] = {
		0, 1, 2,
		2, 3, 0
	};
	
	m_program = Common::Program::create(c_glsl_vs, c_glsl_fs);
	if (m_program == nullptr)
	{
		return false;
	}
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//����ͼƬ
	auto buildinResDir = Util::FileUtil::builtinResDir();
	auto imagePath = Util::FileUtil::combinePath({ buildinResDir, c_pictureName });
	m_texture = new Common::Texture2D(imagePath, GL_RGB);
	m_texture->activeAndBind(0);
	m_program->setUniformInt("wallTexture", 0);
	return true;
}

void textureDemo::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		//ѭ����ʼ�ȴ����¼�
		processEvent();
		glfwMakeContextCurrent(m_window);
		//���������ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//�����ɫ
		glClear(GL_COLOR_BUFFER_BIT);
		m_program->use();
		m_texture->activeAndBind(0);
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//�������������������ʱ�����д��ϵ��£������ҵ�������ӡ
		glfwSwapBuffers(m_window);
		//�����������¼����˴���ȱʧ���ᵼ���������ڴ�����openGL����������Ӧ
		glfwPollEvents();
	}
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	delete m_program;
	delete m_texture;
}

void textureDemo::processEvent()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == true)
	{
		glfwSetWindowShouldClose(m_window, true);
	}
}
