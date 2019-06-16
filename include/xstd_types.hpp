#pragma once

namespace xstd
{
	typedef void * (*allocator)(size_t i_nNumber_To_Allocate);
	typedef void (*deleter)(void * i_pPointer, size_t i_nNumber_To_Deallocate);
}

