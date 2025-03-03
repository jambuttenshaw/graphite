#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>


namespace Graphite
{

	struct Transform
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		glm::mat4 GetLocalToWorld() const;
		glm::mat4 GetWorldToLocal() const;
	};

}
