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
			void apply();
			void reset();

		protected:
			Program();

		protected:
			unsigned int m_program = 0;
		};
	}
}