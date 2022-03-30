#pragma once

namespace OpenGLDemos
{
	namespace Util
	{
		class MathUtil
		{
		public:
			template <class T>
			static T clamp(const T &value, const T &min, const T &max);
		};

		template <class T>
		T MathUtil::clamp(const T &value, const T &min, const T &max)
		{
			if (value < min)
			{
				return min;
			}
			else if (value > max)
			{
				return max;
			}
			else
			{
				return value;
			}
		}
	}
}
