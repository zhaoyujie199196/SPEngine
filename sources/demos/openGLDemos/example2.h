#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;
namespace OpenGLDemos 
{
	namespace Common {
		class Program;
		class Camera;
		class Texture2D;
	}
	/*
	 功能:
	 1. 光照
	 */
	//zhaoyujie TODO 旋转物体时，对着光源的面有些奇怪，不同的面的明暗强度方向不一样
	class Example2
	{
	public:
		static void execute();
		~Example2();
		bool init();
		void run();

	protected:
		bool initImGui();
		void drawImGui();
		void processEvent();
		void updateLightModelMatrix();

	protected:
		bool m_useMaterialMap = true;
		GLFWwindow *m_window = nullptr;
		Common::Camera *m_camera = nullptr;
		glm::mat4 m_projectionMatrix;
		//光源
		unsigned int m_lightVAO = 0;
		unsigned int m_lightVBO = 0;
		Common::Program *m_lightProgram = nullptr;
		glm::mat4 m_lightModelMatrix;
		glm::vec3 m_lightPos = { -5.0, 0.0, 0.0 };
		glm::vec3 m_lightColor;
		//环境光
		float m_ambientStrength = 0.1;
		glm::vec3 m_ambientColor = { 1.0, 1.0, 1.0 };

		//物体
		unsigned int m_objectVAO = 0;
		unsigned int m_objectVBO = 0;
		Common::Program *m_objectProgram = nullptr;
		glm::vec3 m_objectPos = { 2.0f, 0.0f, 0.0f };
		glm::vec3 m_objectRotate = { 0.0f, 45.0f, 0.0f };
		glm::vec3 m_objectColor;
		Common::Texture2D *m_diffuseMap = nullptr;
		Common::Texture2D *m_specularMap = nullptr;
		bool m_useParallelLight = false;
	};
}