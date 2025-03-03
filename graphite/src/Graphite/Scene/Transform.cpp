#include "graphite_pch.h"
#include "Transform.h"

namespace Graphite
{
	glm::mat4 Transform::GetLocalToWorld() const
	{
		return glm::translate(
			glm::identity<glm::mat4>(),
			Translation
		) * glm::eulerAngleXYZ(
			Rotation.x,
			Rotation.y,
			Rotation.z
		) * glm::scale(
			glm::identity<glm::mat4>(),
			Scale
		);
	}

	glm::mat4 Transform::GetWorldToLocal() const
	{
		// TODO: Optimise this
		return glm::inverse(GetLocalToWorld());
	}

}