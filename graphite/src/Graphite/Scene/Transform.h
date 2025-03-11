#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Graphite/Core/Core.h"


namespace Graphite
{

	struct Transform
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		GRAPHITE_API glm::mat4 GetLocalToWorld() const;
		GRAPHITE_API glm::mat4 GetWorldToLocal() const;
	};

}
