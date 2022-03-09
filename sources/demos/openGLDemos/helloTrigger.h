#pragma once

struct GLFWwindow;
namespace OpenGLDemos {
	//绘制openGL三角形的demo
	namespace Common{
		class Program;
	}
	class HelloTrigger 
	{
	public:
		static void execute();
		HelloTrigger() = default;
		~HelloTrigger();
		bool init();
		void run();

	protected:
		void processEvent();

	protected:
		GLFWwindow *m_window = nullptr;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		unsigned int m_vao = 0;
		Common::Program *m_program = nullptr;
		bool m_valid = false;
	};
}
