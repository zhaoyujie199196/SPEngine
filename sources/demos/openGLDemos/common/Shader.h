#pragma once

#include <string>

namespace OpenGLDemos
{
	namespace Common 
	{
		class Shader
		{
			friend class Program;
		public:
			static Shader *create(unsigned int type, const std::string &source);
			~Shader();

		protected:
			Shader(int type);
			bool setSource(const std::string &source);
			int getShaderId() const { return m_shader; }

		protected:
			unsigned int m_shader = 0;
		};
	}
}

