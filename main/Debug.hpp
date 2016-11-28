#pragma once
#ifndef DEBUG_GUARD
#define DEBUG_GUARD

	#ifdef _DEBUG
		#define MD(T)	std::cout << "Debug: " << T << std::endl;
	#else
		#define MD(T)
	#endif
#endif