#include "graphite_pch.h"
#include "Geometry.h"


namespace Graphite
{
	const InputLayout Vertex_Position::VertexInputLayout({
		{"POSITION", 0, 3},
	});

	const InputLayout Vertex_PositionNormalUV::VertexInputLayout({
		{"POSITION", 0, 3},
		{"NORMAL", 0, 3},
		{"TEXCOORD", 0, 2}
	});
}
