#include <string>

namespace OpenGLDemos {
	namespace Common {
		/*
		封装2D图片类
		path : 图片路径
		type : 图片类型 GL_RGB / GL_RGBA
		 */
		class Texture2D
		{
		public:
			Texture2D(const std::string  &path, unsigned int type);
			~Texture2D();
			void bind();
			void activeAndBind(int i);
			int width() const { return m_width; }
			int height() const { return m_height; }
			int channel()  const { return m_channel; }
			unsigned int textureId() const { return m_texture; }

		protected:
			int m_width = 0;
			int m_height = 0;
			int m_channel = 0;
			unsigned int m_texture = 0;
		};
	}
}