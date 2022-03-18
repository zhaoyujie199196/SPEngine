#pragma once

struct GLFWwindow;
namespace OpenGLDemos {
	//ÌùÍ¼demo
	namespace Common {
		class Program;
		class Texture2D;
	}
	class textureDemo
	{
	public:
		static void execute();

		bool init();
		void run();
		void processEvent();
	
	protected:
		GLFWwindow *m_window = nullptr;
		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		Common::Program *m_program = nullptr;
		Common::Texture2D *m_texture = nullptr;
	};
}