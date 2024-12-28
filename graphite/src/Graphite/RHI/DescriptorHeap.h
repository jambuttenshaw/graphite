#pragma once

#include "Graphite/Core/Core.h"


namespace Graphite
{

	class DescriptorHeap
	{
	public:
		DescriptorHeap() = default;
		virtual ~DescriptorHeap() = default;

		// cannot be copied or moved
		DELETE_COPY(DescriptorHeap);
		DEFAULT_MOVE(DescriptorHeap);
	};

}
