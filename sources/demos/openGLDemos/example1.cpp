#include "example1.h"
#include "OpenGLInclude.h"
#include "Defines.h"
#include "common/Program.h"
#include "utils/FileUtil.h"
#include "utils/MathUtil.h"
#include "core/Application.h"
#include "common/Texture2D.h"
#include "common/Camera.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace OpenGLDemos;

static const std::string c_glsl_vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
void main()
{
    TexCoord = aTexCoord;
    gl_Position = vec4(aPos, 1.0f);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * gl_Position;
}
)";
static const std::string c_glsl_fs = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D wallTexture;
uniform sampler2D smileTexture;
uniform float wallRatio;
void main()
{
    FragColor = wallRatio * texture(wallTexture, TexCoord) + (1 - wallRatio) * texture(smileTexture, TexCoord);
}
)";

static const std::string c_wallImageName = "wall.jpg";
static const std::string c_smileImageName = "smile.png";

static const char *c_glslVersion = "#version 330";

static const int c_screenWidth = 800;
static const int c_screenHeight = 600;

static Example1 *s_example1Instance = nullptr;

Example1::~Example1()
{
	glfwMakeContextCurrent(m_window);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	delete m_program;
	delete m_wallTexture;
	delete m_smileTexture;
	delete m_window;
}

void Example1::execute()
{
	Example1 example1;
	s_example1Instance = &example1;
	if (!example1.init())
	{
		return;
	}
	example1.run();
}

bool Example1::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(c_screenWidth, c_screenHeight, "example1", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xPosIn, double yPosIn) {
		s_example1Instance->cursorPositionCallBack(xPosIn, yPosIn);
	});
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow *m_window, int button, int action, int modifies) {
		s_example1Instance->mouseButtonCallBack(button, action, modifies);
	});

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}
	auto initImGuiSuccess = initImGui();
	if (!initImGuiSuccess)
	{
		return false;
	}

	//三角形位置 纹理坐标位置
	float points[] = {
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
	// 
	// 	glGenBuffers(1, &m_ebo);
	// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//加载图片
	auto buildinResDir = Util::FileUtil::builtinResDir();
	auto wallImagePath = Util::FileUtil::combinePath({ buildinResDir, c_wallImageName });
	m_wallTexture = new Common::Texture2D(wallImagePath, GL_RGB);
	m_wallTexture->bind();
	auto smileImagePath = Util::FileUtil::combinePath({ buildinResDir, c_smileImageName });
	m_smileTexture = new Common::Texture2D(smileImagePath, GL_RGBA);
	m_smileTexture->bind();

	m_projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(c_screenWidth) / c_screenHeight, 0.1f, 100.0f);
	m_camera = new Common::Camera();
	m_camera->setPosition({ 0.0f, 0.0f, 5.0f });
	m_camera->setFront({ 0.0f, 0.0f, -1.0f });
	m_camera->setUpWorld({ 0.0, 1.0, 0.0 });
	return true;
}

bool Example1::initImGui()
{
	//初始化imgui
	if (!IMGUI_CHECKVERSION())
	{
		return false;
	}
	if (ImGui::CreateContext() == nullptr)
	{
		return false;
	}

	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true))
	{
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init(c_glslVersion))
	{
		return false;
	}
	return true;
}

void Example1::run()
{
	m_lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(m_window))
	{
		m_renderTime = m_renderTime + glfwGetTime() - m_lastTime;
		//循环开始先处理事件
		processEvent();
		glfwMakeContextCurrent(m_window);
		drawTexture();
		drawImGui();
		//交换缓冲区，否则绘制时，会有从上到下，从左到右的线条打印
		glfwSwapBuffers(m_window);
		//处理鼠标键盘事件。此代码缺失，会导致鼠标键盘在创建的openGL窗口上无响应
		glfwPollEvents();
		m_lastTime = m_renderTime;
	}
}

void Example1::drawTexture()
{
	static glm::vec3 s_cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	static glm::vec3 s_cubeRotate[10] = {};
	static bool s_initCube = false;
	if (!s_initCube)
	{
		s_initCube = true;
		for (int i = 0; i < 10; ++i)
		{
			s_cubeRotate[i] = glm::vec3(std::rand() % 360, std::rand() % 360, std::rand() % 360);
		}
	}

	//auto curTimeView = glfwGetTime();
	//m_camera->setCameraPos(glm::vec3(sin(curTimeView) * m_viewDepth, 0.0f, cos(curTimeView) * m_viewDepth));
	const glm::mat4 &viewMatrix = m_camera->viewMatrix();

	//设置清除颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//清除颜色
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_program->use();
	m_wallTexture->activeAndBind(0);
	m_smileTexture->activeAndBind(1);
	m_program->setUniformInt("wallTexture", 0);
	m_program->setUniformInt("smileTexture", 1);
	m_program->setUniformFloat("wallRatio", m_wallRatio);

	// 	// 跟随时间旋转
	// 	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	// 	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	// 	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	// 	m_program->setUniformMatrix4F("transMatrix", transform);

	m_program->setUniformMatrix4f("viewMatrix", viewMatrix);
	m_program->setUniformMatrix4f("projectionMatrix", m_projectionMatrix);

	glBindVertexArray(m_vao);
	for (int i = 0; i < 10; ++i)
	{
		glm::mat4 modelMatrix(1.0f);
		const auto &oriRotate = s_cubeRotate[i];
		auto curTimeAngle = (float)glfwGetTime() * 5.0f;
		modelMatrix = glm::translate(modelMatrix, s_cubePositions[i]);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_modelRotate[0] + oriRotate[0] + curTimeAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_modelRotate[1] + oriRotate[1] + curTimeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_modelRotate[2] + oriRotate[2] + curTimeAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		m_program->setUniformMatrix4f("modelMatrix", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Example1::drawImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("EditArea");
	ImGui::SliderFloat("mixRatio", &m_wallRatio, 0, 1);
	ImGui::SliderFloat3("rotate", m_modelRotate, 0.0f, 360.0f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Example1::processEvent()
{
	float deltaTime = glfwGetTime() - m_lastTime;
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_camera->move(Common::Camera::Forward, deltaTime);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_camera->move(Common::Camera::Backward, deltaTime);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_camera->move(Common::Camera::Left, deltaTime);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_camera->move(Common::Camera::Right, deltaTime);
	}
}

void Example1::cursorPositionCallBack(double xPosIn, double yPosIn)
{
	if (m_rotateState)
	{
		if (!m_rotateInited)
		{
			m_rotateInited = true;
		}
		else
		{
			glm::vec2 offset = {xPosIn - m_lastMouseX, yPosIn - m_lastMouseY};
			m_camera->rotate(offset);
		}
		m_lastMouseX = xPosIn;
		m_lastMouseY = yPosIn;
	}
}

void Example1::mouseButtonCallBack(int button, int action, int modifies)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		if (action == GLFW_PRESS)
		{
			m_rotateState = true;
			m_rotateInited = false;
		}
		else if (action == GLFW_RELEASE)
		{
			m_rotateState = false;
		}
	}
}

