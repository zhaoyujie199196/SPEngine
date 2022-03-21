#pragma once

struct GLFWwindow;
namespace OpenGLDemos {
	//贴图demo
	namespace Common {
		class Program;
		class Texture2D;
	}
	class textureDemo
	{
	public:
		static void execute();

	protected:
		bool init();
		bool initImGui();
		void run();
		void processEvent();
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
	};
}