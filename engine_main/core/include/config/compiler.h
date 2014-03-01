#ifndef __COMPILER_H__
#define __COMPILER_H__

#if defined(_MSC_VER)

	#define SE_COMPILER msvc
	#define SE_COMPILER_MSVC 1

#elif defined(GCC) || defined(__GCC__) || defined(__ANDROID__)

	#define SE_COMPILER gcc
	#define SE_COMPILER_GCC 1 

#endif

#define SE_COMPILER_HEADER(path) SE_COMPILER_HEADER2(path, SE_COMPILER)
#define SE_COMPILER_HEADER1(path, compiler) SE_COMPILER_HEADER2(path, compiler)
#define SE_COMPILER_HEADER2(path, compiler) SE_COMPILER_HEADER3(path, compiler)
#define SE_COMPILER_HEADER3(path, compiler) <path##_##compiler##_.h>

#include SE_COMPILER_HEADER(config/compiler)


#endif 