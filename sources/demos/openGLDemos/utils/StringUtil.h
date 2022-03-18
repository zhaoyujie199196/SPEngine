#pragma once

#include <string>

namespace OpenGLDemos {
	namespace Util {
		class StringUtil
		{
		public:
			//Ìæ»»×Ö·û´®
			static std::string replace(const std::string &s, const std::string &before, const std::string &after);
		};
	}

}