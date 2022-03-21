#pragma once

#include <string>
#include <glm/glm.hpp>

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
			void setUniformFloat(const std::string &uniformName, float value);
			void setUniformMatrix4F(const std::string &uniformName, const glm::mat4 &matrix);

		protected:
			Program();

		protected:
			unsigned int m_program = 0;
		};
	}
}