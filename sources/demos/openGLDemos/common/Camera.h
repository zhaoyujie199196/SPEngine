#pragma once

#include <glm/glm.hpp>
#include "Defines.h"

namespace OpenGLDemos 
{
	namespace Common 
	{
		class Camera 
		{
		public:
			enum EnCameraMoveDirection {
				Forward = 0,
				Backward,
				Left,
				Right
			};

			Camera();
			Camera(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &upWorld);
			void setPosition(const glm::vec3 &position);
			void setFront(const glm::vec3 &front);
			void setUpWorld(const glm::vec3 &upWorld);
			void setMoveSpeed(float moveSpeed);
			void setRotateSpeed(float rotateSpeed);
			const glm::vec3 &position() const;
			const glm::vec3 &front() const;
			const glm::vec3 &upWorld() const;
			const glm::mat4 &viewMatrix();
			void move(EnCameraMoveDirection direction, float deltaTime);
			void rotate(const glm::vec2 &offset);
			void setDirty();

		protected:
			void update();
			void updateEulerAngles();

		protected:
			glm::vec3 m_position;    //���λ��
			glm::vec3 m_front;       //�������
			glm::vec3 m_upWorld;     //�Ϸ���
			glm::mat4 m_viewMatrix;  //���view����
			float m_moveSpeed = 1.0f;
			float m_rotateSpeed = 0.0015f;
			bool m_dirty = true;            //matrix�Ƿ���Ҫ��������
			float m_pitch = 0;
			float m_yaw = -OD_PI_2;
		};
	}
}