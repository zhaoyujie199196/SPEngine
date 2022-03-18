#pragma once

#include <string>

namespace OpenGLDemos
{
	namespace Common
	{
		class Program 
		{
		public:
			static Program* create(const std::string &vs, std::string &gs, std::string &fs);
			static Program *create(const std::string &vs, const std::string &fs);
			~Program();
			void use();
			void reset();
			unsigned int programID() const;
			void setUniformInt(const std::string &uniformName, int value);

		protected:
			Program();

		protected:
			unsigned int m_program = 0;
		};
	}
}