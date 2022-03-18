#pragma once

#include <string>

//定义宏方便取用
#define OD_APP OpenGLDemos::Core::Application::instance()

namespace OpenGLDemos {
	namespace Core{
		struct ApplicationPrivate;
		class Application {
		public:
			Application(int argc, char **argv);
			virtual ~Application();
			static Application *instance();
			const std::string &applicationDir();
			const std::string &applicationPath();

		private:
			ApplicationPrivate *p_ptr() const;
			void init();

		private:
			ApplicationPrivate *m_p = nullptr;
			static Application *m_self;
		};
	}
}