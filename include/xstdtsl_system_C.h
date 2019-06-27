#pragma once
#if defined _WIN32 || defined _WIN64
#define __XSTDTSL_EXPORT __declspec(dllexport)
#else
#define __XSTDTSL_EXPORT
#endif

extern "C"
{
	__XSTDTSL_EXPORT size_t xstdtsl_get_word_size(void) noexcept;
	__XSTDTSL_EXPORT size_t xstdtsl_get_available_memory(void) noexcept;
}


#undef __XSTDTSL_EXPORT
