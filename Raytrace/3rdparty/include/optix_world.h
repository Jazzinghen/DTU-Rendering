
/*
 * Copyright (c) 2010 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and proprietary
 * rights in and to this software, related documentation and any modifications thereto.
 * Any use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from NVIDIA Corporation is strictly
 * prohibited.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS*
 * AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY
 * SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
 * BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
 * INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES
 */

#ifndef __optixu_optixu_world_h__
#define __optixu_optixu_world_h__

/*
 * This header is designed to be included by both host and device code providing access to
 * the C-API along with the C++ API found in optixpp_namespaces.h.  In addition various
 * helper classes and file will also be included when compiling C++ compatible code.
 *
 * Note that the CUDA vector types will be defined in the optix:: namespace.
 */

#ifndef RTAPI
#if defined( _WIN32 )
#define RTAPI __declspec(dllimport)
#else
#define RTAPI
#endif
#endif

#include "internal/optix_declarations.h"

#if defined( _WIN64 )
typedef unsigned __int64    RTsize;
#elif defined( _WIN32 )
typedef unsigned int        RTsize;
#else
typedef long unsigned int   RTsize;
#endif

#include "optixu/optixu_vector_types.h"
#include "optixu/optixu_vector_functions.h"

/* None of these headers are compatible with C */
#if defined(__cplusplus)
#  include "optixu/optixu_math_namespace.h"
#  include "optixu/optixu_aabb_namespace.h"
#  include "optixu/optixu_matrix_namespace.h"
#  include "optixu/optixu_math_stream_namespace.h"
#endif

#endif /* #ifndef __optixu_optixu_world_h__ */
