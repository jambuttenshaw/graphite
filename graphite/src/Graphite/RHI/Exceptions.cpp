#include "pch.h"
#include "Exceptions.h"

#include <comdef.h>


namespace Graphite
{

	std::wstring DXException::ToString() const
	{
		return _com_error(ErrorCode).ErrorMessage();
	}

}
