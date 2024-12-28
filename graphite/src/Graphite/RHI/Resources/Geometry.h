#pragma once

#include "InputLayout.h"


namespace Graphite
{
	struct Vertex_Position
	{
		glm::vec3 Position;

		// Input layout for this vertex type
		// TODO: build these from shader reflection
		// TODO: Cache these so they can be shared by compatible vertex types
		static const InputLayout VertexInputLayout;
	};

	// Standard vertex types
	struct Vertex_PositionNormalUV
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;

		// Input layout for this vertex type
		// TODO: build these from shader reflection
		// TODO: Cache these so they can be shared by compatible vertex types
		static const InputLayout VertexInputLayout;
	};
}
