#pragma once

#include <comdef.h>


struct DXException : public std::exception
{
	HRESULT ErrorCode = S_OK;

	DXException() = default;
	DXException(HRESULT hr)
		: ErrorCode(hr)
	{}

	std::wstring ToString() const
	{
		return _com_error(ErrorCode).ErrorMessage();
	}
};


#ifdef _DEBUG

#define THROW_IF_FAIL(x) { const HRESULT hr = x; if (FAILED(hr)) { LOG_FATAL(DXException(hr).ToString().c_str()); throw; }}
#define ASSERT(x, msg) if (!(x)) { LOG_ERROR("Debug assertion failed in file ({0}) on line ({1}). Message: {2}", __FILE__, __LINE__, msg); DebugBreak(); }

#define NOT_IMPLEMENTED ASSERT(false, "Not Implemented")
#define DEPRECATED ASSERT(false, "Deprecated")

#else

#define THROW_IF_FAIL(x) (void)(x);
#define ASSERT(x, msg)

#define NOT_IMPLEMENTED
#define DEPRECATED

#endif
