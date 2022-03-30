#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Defines.h"
#include "utils/MathUtil.h"

using namespace OpenGLDemos::Common;

Camera::Camera()
	: m_position(0.0f, 0.0f, 1.0f)
	, m_front(0.0f, 0.0f, -1.0f)
	, m_upWorld(0.0f, 1.0f, 0.0f)
{
	updateEulerAngles();
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &upWorld)
	: m_position(position)
	, m_front(front)
	, m_upWorld(upWorld)
{
	updateEulerAngles();
}

void Camera::setPosition(const glm::vec3 &position)
{
	m_position = position;
	setDirty();
}

void Camera::setFront(const glm::vec3 &front)
{
	if (front == glm::vec3(0.0, 0.0, 0.0))
	{
		OD_Assert(false);
		return;
	}
	m_front = front;
	updateEulerAngles();
	setDirty();
}

void Camera::setUpWorld(const glm::vec3 &upWorld)
{
	m_upWorld = upWorld;
	setDirty();
}

void Camera::setMoveSpeed(float moveSpeed)
{
	m_moveSpeed = moveSpeed;
}

void Camera::setRotateSpeed(float rotateSpeed)
{
	m_rotateSpeed = rotateSpeed;
}

const glm::vec3 &Camera::position() const
{
	return m_position;
}

const glm::vec3& Camera::front() const
{
	return m_front;
}

const glm::vec3 &Camera::upWorld() const
{
	return m_upWorld;
}

const glm::mat4 &Camera::viewMatrix()
{
	if (m_dirty)
	{
		update();
		m_dirty = false;
	}
	return m_viewMatrix;
}

void Camera::setDirty()
{
	m_dirty = true;
}

void Camera::move(EnCameraMoveDirection direction, float deltaTime)
{
	auto normalizeFront = glm::normalize(m_front);
	auto normlizeRight = glm::normalize(glm::cross(m_front, m_upWorld));
	if (direction == Camera::Forward)
	{
		m_position -= normalizeFront * m_moveSpeed * deltaTime;
	}
	else if (direction == Camera::Backward)
	{
		m_position += normalizeFront * m_moveSpeed * deltaTime;
	}
	else if (direction == Camera::Left)
	{
		m_position -= normlizeRight * m_moveSpeed * deltaTime;
	}
	else if (direction == Camera::Right)
	{
		m_position += normlizeRight * m_moveSpeed * deltaTime;
	}
	setDirty();
}

void Camera::rotate(const glm::vec2 &offset)
{
	m_pitch += offset[1] * m_rotateSpeed;
	m_pitch = OpenGLDemos::Util::MathUtil::clamp<float>(m_pitch, -OD_PI_2, OD_PI_2);
	m_yaw -= offset[0] * m_rotateSpeed;
	glm::vec3 front;
	front.x = cos(m_yaw) * cos(m_pitch);
	front.y = sin(m_pitch);
	front.z = sin(m_yaw) * cos(m_pitch);
	m_front = glm::normalize(front);
	setDirty();
}

void Camera::update()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_upWorld);
}

void Camera::updateEulerAngles()
{
	m_pitch = glm::asin(m_front[1] / glm::length(m_front));
	auto yawFaceVec = glm::vec2(m_front[0], -m_front[2]);
	auto yawLength = glm::length(yawFaceVec);
	if (yawLength == 0)
	{
		return;
	}
	m_yaw = -glm::acos(yawFaceVec[0] / yawLength);
}