#pragma once
#include "Core/Core.h"


namespace Graphite
{

	class GRAPHITE_API RendererInterface
	{
	public:
		virtual ~RendererInterface() = 0;

		DEFAULT_COPY(RendererInterface);
		DEFAULT_MOVE(RendererInterface);

		// Any renderer to be used with Graphite should implement these interfaces

		// To allow uploads / resource configuration before the frame has rendered
		virtual void PreRender() {}

		// Perform rendering work
		virtual void Render() {}

		// Tidy up after a frame has rendered
		virtual void PostRender() {}
	};

}
