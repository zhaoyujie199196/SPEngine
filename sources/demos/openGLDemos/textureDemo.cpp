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
	
	//三角形位置 纹理坐标位置
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

	//加载图片
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
		//循环开始先处理事件
		processEvent();
		glfwMakeContextCurrent(m_window);
		//设置清除颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清除颜色
		glClear(GL_COLOR_BUFFER_BIT);
		m_program->use();
		m_texture->activeAndBind(0);
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//交换缓冲区，否则绘制时，会有从上到下，从左到右的线条打印
		glfwSwapBuffers(m_window);
		//处理鼠标键盘事件。此代码缺失，会导致鼠标键盘在创建的openGL窗口上无响应
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
