#include "example2.h"
#include "OpenGLInclude.h"
#include "common/Program.h"
#include "common/Camera.h"
#include "common/Texture2D.h"
#include "utils/FileUtil.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

using namespace OpenGLDemos;

static const std::string c_glsl_light_vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() 
{
    gl_Position = vec4(aPos, 1.0f);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * gl_Position;
}
)";

static const std::string c_glsl_light_fs = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 lightColor;
void main() 
{
    FragColor = vec4(lightColor, 1.0f);
}
)";

static const std::string c_glsl_object_vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 NormalVec;
out vec3 FragPos;

void main() 
{
    gl_Position = vec4(aPos, 1.0f);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * gl_Position;
    NormalVec = aNormal;
    FragPos = aPos;
}
)";

static const std::string c_glsl_object_fs = R"(
#version 330 core
out vec4 FragColor;
in vec3 NormalVec;
in vec3 FragPos;
uniform mat4 modelMatrix;
//点光源
uniform vec3 lightPos;
//摄像机位置
uniform vec3 cameraPos;
uniform float specularStrength;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    vec3 norm = mat3(transpose(inverse(modelMatrix))) * NormalVec;
    norm = normalize(norm);
    vec3 lightDir = normalize(lightPos - FragPos);
    //环境光
    vec3 ambientColor = material.ambient * light.ambient;
    //漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = material.diffuse * diff * light.diffuse;
    //镜面反射
    vec3 cameraDir = normalize(cameraPos - FragPos);
    float specular = pow(max(dot(reflect(-lightDir, norm), cameraDir), 0), material.shininess);
    vec3 specularColor = specular * material.specular * light.specular;

    vec3 color = ambientColor + diffuseColor + specularColor;
    FragColor = vec4(color, 1.0f);
}
)";

static const std::string c_glsl_object_materialMap_vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 NormalVec;
out vec3 FragPos;
out vec2 TexCoord;

void main() 
{
    gl_Position = vec4(aPos, 1.0f);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * gl_Position;
    NormalVec = aNormal;
    FragPos = aPos;
    TexCoord = aTexCoord;
}
)";

static const std::string c_glsl_object_materialMap_fs = R"(
#version 330 core
out vec4 FragColor;
in vec3 NormalVec;
in vec3 FragPos;
in vec2 TexCoord;
uniform mat4 modelMatrix;
//点光源
uniform vec3 lightPos;
//摄像机位置
uniform vec3 cameraPos;
uniform float specularStrength;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};
uniform Material material;
uniform float useParallelLight;

struct Light {
    vec3 parallelLightDir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //点光源衰减
    float constant;   //衰减常量
    float linear;     //衰减一次方量
    float quadratic;  //衰减二次方量
};
uniform Light light;

void main()
{
    vec3 norm = mat3(transpose(inverse(modelMatrix))) * NormalVec;
    norm = normalize(norm);
    vec3 lightDir;
    float attenuation = 1.0f;  //光照距离衰减
    if (useParallelLight > 0.5f)
    {
        lightDir = normalize(lightPos - FragPos);
    }
    else
    {
        lightDir = normalize(-light.parallelLightDir);
        float distance = length(lightPos - FragPos);
        attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    }
    vec3 diffuseMapColor = vec3(texture(material.diffuseMap, TexCoord));
    vec3 specularMapColor = vec3(texture(material.specularMap, TexCoord));
    //环境光
    vec3 ambientColor = diffuseMapColor * light.ambient * attenuation;
    //漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diffuseMapColor * diff * light.diffuse * attenuation;
    //镜面反射
    vec3 cameraDir = normalize(cameraPos - FragPos);
    float specular = pow(max(dot(reflect(-lightDir, norm), cameraDir), 0), material.shininess);
    vec3 specularColor = specular * specularMapColor * light.specular * attenuation;

    vec3 color = ambientColor + diffuseColor + specularColor;
    FragColor = vec4(color, 1.0f);
}
)";

const int c_windowWidth = 800;
const int c_windowHeight = 600;
const char *c_glslVersion = "#version 330";
const std::string c_diffuseMapPath = "material/diffuseMap.png";
const std::string c_specularMapPath = "material/specularMap.png";

const float c_CubePoints[] = {
	// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


Example2::~Example2()
{
	glfwMakeContextCurrent(m_window);
	delete m_camera;
	glDeleteBuffers(1, &m_lightVBO);
	glDeleteBuffers(1, &m_objectVBO);
	delete m_lightProgram;
	delete m_objectProgram;
	delete m_diffuseMap;
	delete m_specularMap;
}

void Example2::execute()
{
	Example2 example2;
	if (!example2.init())
	{
		return;
	}
	example2.run();
}

bool Example2::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(c_windowWidth, c_windowHeight, "exapmle2", NULL, NULL);
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(c_windowWidth) / c_windowHeight, 0.1f, 100.0f);
	//m_projectionMatrix = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	m_camera = new Common::Camera;
	m_camera->setPosition({ 0.0f, 0.0f, 10.0f });
	m_camera->setFront({ 0.0f, 0.0f, -5.0f });
	m_camera->setUpWorld({ 0.0f, 1.0f, 0.0f });

	glGenVertexArrays(1, &m_lightVAO);
	glBindVertexArray(m_lightVAO);

	glGenBuffers(1, &m_lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(c_CubePoints), c_CubePoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	m_lightProgram = Common::Program::create(c_glsl_light_vs, c_glsl_light_fs);
	if (m_lightProgram == nullptr)
	{
		return false;
	}

	m_lightColor = { 1.0f, 1.0f, 1.0f };

	glGenVertexArrays(1, &m_objectVAO);
	glBindVertexArray(m_objectVAO);
	glGenBuffers(1, &m_objectVBO);
	glBindBuffer(GL_VERTEX_ARRAY, m_objectVBO);
	glBufferData(GL_VERTEX_ARRAY, sizeof(c_CubePoints), c_CubePoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	if (m_useMaterialMap)
	{
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	if (m_useMaterialMap)
	{
		m_objectProgram = Common::Program::create(c_glsl_object_materialMap_vs, c_glsl_object_materialMap_fs);
	}
	else
	{
		m_objectProgram = Common::Program::create(c_glsl_object_vs, c_glsl_object_fs);
	}
	if (m_objectProgram == nullptr)
	{
		return false;
	}
	m_objectColor = { 1.0, 0.5, 0.2 };

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	auto diffuseMapPath = Util::FileUtil::combinePath({Util::FileUtil::builtinResDir(), c_diffuseMapPath});
	m_diffuseMap = new Common::Texture2D(diffuseMapPath, GL_RGBA);
	m_diffuseMap->bind();
	auto specularMapPath = Util::FileUtil::combinePath({Util::FileUtil::builtinResDir(), c_specularMapPath});
	m_specularMap = new Common::Texture2D(specularMapPath, GL_RGBA);
	m_specularMap->bind();
	if (!initImGui())
	{
		return false;
	}

	return true;
}

bool Example2::initImGui()
{
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

void Example2::run()
{
	static glm::vec3 s_cubePositions[] = {
	    glm::vec3(-4.0f,  0.0f,  0.0f),
	    glm::vec3(2.0f,  0.0f,  0.0f),
	    glm::vec3(4.5f,  0.0f,  0.0f),
	    glm::vec3(1.8f, -2.0f, -12.3f),
	    glm::vec3(5.4f, -0.4f, -3.5f),
	    glm::vec3(7.7f,  3.0f, -7.5f),
	    glm::vec3(4.3f, -2.0f, -2.5f),
	    glm::vec3(3.5f,  2.0f, -2.5f),
	    glm::vec3(6.5f,  0.2f, -1.5f),
	    glm::vec3(1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(m_window))
	{
		processEvent();
		glfwMakeContextCurrent(m_window);
		updateLightModelMatrix();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(m_lightVAO);
		m_lightProgram->use();
		m_lightProgram->setUniformMatrix4f("modelMatrix", m_lightModelMatrix);
		m_lightProgram->setUniformMatrix4f("viewMatrix", m_camera->viewMatrix());
		m_lightProgram->setUniformMatrix4f("projectionMatrix", m_projectionMatrix);
		m_lightProgram->setUniformVec3f("lightColor", m_lightColor);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(m_objectVAO);
		m_objectProgram->use();
		m_objectProgram->setUniformMatrix4f("viewMatrix", m_camera->viewMatrix());
		m_objectProgram->setUniformMatrix4f("projectionMatrix", m_projectionMatrix);
		m_objectProgram->setUniformVec3f("lightPos", m_lightPos);
		m_objectProgram->setUniformVec3f("light.ambient", m_lightColor * 0.1f);
		m_objectProgram->setUniformVec3f("light.diffuse", m_lightColor * 0.5f);
		m_objectProgram->setUniformVec3f("light.specular", m_lightColor);
		m_objectProgram->setUniformVec3f("cameraPos", m_camera->position());
		m_objectProgram->setUniformVec3f("light.parallelLightDir", {1.0f, 0.0f, 0.0f});
		m_objectProgram->setUniformFloat("useParallelLight", m_useParallelLight ? 1.0f : 0.0f);
		m_objectProgram->setUniformFloat("light.constant", 1.0f);
		m_objectProgram->setUniformFloat("light.linear", 0.09f);
		m_objectProgram->setUniformFloat("light.quadratic", 0.032f);

		if (m_useMaterialMap)
		{
			m_diffuseMap->activeAndBind(0);
			m_specularMap->activeAndBind(1);
			m_objectProgram->setUniformInt("material.diffuseMap", 0);
			m_objectProgram->setUniformInt("material.specularMap", 1);
		}
		else
		{
			m_objectProgram->setUniformVec3f("material.ambient", m_objectColor);
			m_objectProgram->setUniformVec3f("material.diffuse", m_objectColor);
			m_objectProgram->setUniformVec3f("material.specular", { 1.0f, 1.0f, 1.0f });
		}
		for (int i = 0; i < 10; ++i)
		{
			auto objectModelMatrix = glm::mat4(1.0f);
			objectModelMatrix = glm::translate(objectModelMatrix, s_cubePositions[i]);
			objectModelMatrix = glm::rotate(objectModelMatrix, glm::radians(m_objectRotate[0]), glm::vec3(1.0f, 0.0f, 0.0f));
			objectModelMatrix = glm::rotate(objectModelMatrix, glm::radians(m_objectRotate[1]), glm::vec3(0.0f, 1.0f, 0.0f));
			objectModelMatrix = glm::rotate(objectModelMatrix, glm::radians(m_objectRotate[2]), glm::vec3(0.0f, 0.0f, 1.0f));
			if (i > 2)
			{
				float angle = 20.0f * i;
				objectModelMatrix = glm::rotate(objectModelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			m_objectProgram->setUniformMatrix4f("modelMatrix", objectModelMatrix);
			m_objectProgram->setUniformFloat("material.shininess", 32.0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		drawImGui();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void Example2::updateLightModelMatrix()
{
	m_lightModelMatrix = glm::mat4(1.0f);
	m_lightModelMatrix = glm::translate(m_lightModelMatrix, m_lightPos);
	m_lightModelMatrix = glm::scale(m_lightModelMatrix, { 0.1, 0.1, 0.1 });
}

void Example2::processEvent()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}
}

void Example2::drawImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("EditArea");
	ImGui::ColorEdit3("lightColor", &m_lightColor[0]);
	ImGui::SliderFloat3("lightPos", &m_lightPos[0], -5.0, 5.0);
	ImGui::ColorEdit3("objectColor", &m_objectColor[0]);
	ImGui::SliderFloat3("objectRotate", &m_objectRotate[0], 0.0f, 360.0f);
	ImGui::SliderFloat("ambientStrength", &m_ambientStrength, 0.0f, 1.0f);
	ImGui::ColorEdit3("ambientColor", &m_ambientColor[0]);
	ImGui::Checkbox("useParallel", &m_useParallelLight);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}