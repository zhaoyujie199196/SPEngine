#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;
namespace OpenGLDemos {
	/*
	 功能：
	 1. 2D贴图
	 2. ImGui与联动
	 3. 与输入设备交互
	 4. 模型矩阵、视图矩阵、投影矩阵
	 */
	namespace Common {
		class Program;
		class Texture2D;
		class Camera;
	}
	class Example1
	{
	public:
		static void execute();
		~Example1();

	protected:
		bool init();
		bool initImGui();
		void run();
		void processEvent();
		void cursorPositionCallBack(double xPosIn, double yPosIn);
		void mouseButtonCallBack(int button, int action, int modifies);
		void drawTexture();
		void drawImGui();
	
	protected:
		GLFWwindow *m_window = nullptr;
		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		float m_wallRatio = 0.5;
		Common::Program *m_program = nullptr;
		Common::Texture2D *m_wallTexture = nullptr;
		Common::Texture2D * m_smileTexture = nullptr;
		Common::Camera *m_camera = nullptr;
		glm::mat4 m_projectionMatrix;
		float m_modelRotate[3] = { 0.0f, 0.0f, 0.0f };
		double m_renderTime = 0.0f;
	    double m_lastTime = 0.0f;
		bool m_rotateState = false;
		double m_lastMouseX = 0;
		double m_lastMouseY = 0;
		double m_rotateInited = false;
	};
}