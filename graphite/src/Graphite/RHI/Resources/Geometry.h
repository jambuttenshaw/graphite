#pragma once

#include "Graphite/Core/Core.h"
#include "InputLayout.h"


namespace Graphite
{
	struct Vertex_Position
	{
		glm::vec3 Position;

		// Input layout for this vertex type
		// TODO: build these from shader reflection
		// TODO: Cache these so they can be shared by compatible vertex types
		GRAPHITE_API static const InputLayout VertexInputLayout;
	};

	struct Vertex_PositionNormal
	{
		glm::vec3 Position;
		glm::vec3 Normal;

		// Input layout for this vertex type
		// TODO: build these from shader reflection
		// TODO: Cache these so they can be shared by compatible vertex types
		GRAPHITE_API static const InputLayout VertexInputLayout;
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
		GRAPHITE_API static const InputLayout VertexInputLayout;
	};
}
