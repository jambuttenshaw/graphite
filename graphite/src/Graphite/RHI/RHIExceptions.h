#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/Core/Log.h"


// Error and exception handling

namespace Graphite
{
	struct DXException : public std::exception
	{
		HRESULT ErrorCode = S_OK;

		DXException() = default;
		DXException(HRESULT hr)
			: ErrorCode(hr)
		{
		}

		std::wstring ToString() const;
	};
}

#ifdef GRAPHITE_DEBUG
#define DX_THROW_IF_FAIL(x) { const HRESULT hr = x; if (FAILED(hr)) { GRAPHITE_LOG_FATAL(DXException(hr).ToString().c_str()); }}
#define DX_LOG_HR_ERROR(x) GRAPHITE_LOG_ERROR(DXException(x).ToString().c_str())
#else
#define DX_THROW_IF_FAIL(x) (void)(x);
#define DX_LOG_HR_ERROR(x) 
#endif
