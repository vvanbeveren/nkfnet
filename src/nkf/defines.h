/*
 * defines.h
 *
 *  Created on: 8 nov. 2011
 *      Author: vincentb
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#ifdef _WIN32
#define	WIN32_API
#endif

#ifdef WIN32_API

#ifdef NKFNET_EXP_DLL

#define NKFNET_API	__declspec(dllexport)

#else

#ifdef NKFNET_IMP_DLL
#define NKFNET_API	__declspec(import)
#else
#define NKFNET_API
#endif
#endif

#else

#define	NKFNET_API

#endif

#endif /* DEFINES_H_ */
