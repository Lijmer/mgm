////////////////////////////////////////////////////////////////////////////////
//MIT License
//
//Copyright(c) 2017 Max Oomen
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
////////////////////////////////////////////////////////////////////////////////


//title: mgm.h
//author: Max Oomen
//e-mail: mjs.oomen@gmail.com
//description: 'Minimal Game/Graphics Math', a C and C++ Math library.

//The goal of this math library is to be a simple library that is still fairly
// complete feature wise. All the main functionallity is available when compiled
// using a C compiler. When compiled with C++ function overloading and operator
// overloading are added.
//Both left handed and right handed matrices are supported. All matrix math is
// done in a column major fashion.

//It has only been compiled with Visual Studio 2015's C and C++ compilers
//It has also been designed with fast compilation in mind, since a short 
// iteration time is key to productivity. That's why there are no templates :)

//There are still some features lacking, since I add features as I need them.

//You can define MGM_Z_ZERO_TO_ONE to make projection matrices use a Z from 0 to
// 1, instead of the default -1 to 1.

#ifndef INCLUDED_MGM_H
#define INCLUDED_MGM_H

#include <math.h>

#if defined(_MSVC)
#  pragma warning(push)
#  pragma warning(disable : 4201)
#  pragma warning(disable : 4204)
#  define MGM_INLINE static __forceinline
#elif defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wmissing-braces"
#  pragma clang diagnostic ignored "-Wunused-function"
#  define MGM_INLINE static __attribute__((always_inline))
#else
#  define MGM_INLINE static inline
#endif

#ifndef min
#  define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#  define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define MGM_SHUF2(x, y) (((x) & 0x1) | (((y) & 0x1) << 1))
#define MGM_SHUF3(x, y, z) (((x) & 0x3)       |                                \
                           (((y) & 0x3) << 2) |                                \
                           (((z) & 0x3) << 4))
#define MGM_SHUF4(x, y, z, w) (((x) & 0x3)       |                             \
                              (((y) & 0x3) << 2) |                             \
                              (((z) & 0x3) << 4) |                             \
                              (((w) & 0x3) << 6))

typedef struct float2
{
#ifdef __cplusplus
	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }
#endif

	union
	{
		float v[2];
		struct { float x, y; };
	};
} float2;

typedef struct float3
{
#ifdef __cplusplus
	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }
#endif
	union
	{
		float v[3];
		struct { float x, y, z; };

		//fancy common access patterns
		struct { float2 xy; float ___padding0; };
		struct { float ___padding1; float2 yz; };
	};
}float3;

typedef struct float4
{
#ifdef __cplusplus
	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }
#endif
	union
	{
		float v[4];
		struct { float x, y, z, w; };

		//fancy common access patterns
		struct { float ___padding0; float2 yz; float ___padding1; };
		struct { float2 xy; float2 zw; };
		struct { float3 xyz; float ___padding4; };
		struct { float ___padding5; float3 yzw; };
	};
}float4;

typedef struct quatf
{
#ifdef __cplusplus
	float operator[](int idx) const { return v[idx]; }
	float& operator[](int idx) { return v[idx]; }
#endif
	union
	{
		float v[4];
		struct { float i, j, k, r; };
		struct { float x, y, z, w; };

		//fancy common access patterns
		float4 xyzw;
		struct { float2 xy; float2 zw; };
		struct { float ___padding0; float2 yz; float ___padding1; };
		struct { float3 xyz; float ___padding4; };
		struct { float ___padding5; float3 yzw; };
	};
}quatf;

typedef struct float2x2
{
#ifdef __cplusplus
	float2 operator[](int i) const { return col[i]; }
	float2& operator[](int i) { return col[i]; }
#endif
	union
	{
		struct { float2 x, y; };
		float2 col[2];
		float a[4];
		float v[2][2];
	};
}float2x2;

typedef struct float3x3
{
#ifdef __cplusplus
	float3 operator[](int i) const { return col[i]; }
	float3& operator[](int i) { return col[i]; }
#endif
	union
	{
		struct { float3 x, y, z; };
		float3 col[3];
		float v[3][3];
		float a[9];
	};
}float3x3;

typedef struct float4x4
{
#ifdef __cplusplus
	float4 operator[](int i) const { return col[i]; }
	float4& operator[](int i) { return col[i]; }
#endif
	union
	{
		struct { float4 x, y, z, w; };
		float4 col[4];
		float v[4][4];
		float a[16];
	};
}float4x4;


////////////////////////////////////////////////////////////////////////////////
//Make
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 make1f2(float x)         { return { x, x }; }
MGM_INLINE float2 makef2(float x, float y) { return { x, y }; }

#ifdef __cplusplus
MGM_INLINE float2 makef2(float x) { return make1f2(x); }
#endif

MGM_INLINE float3 make1f3(float x) { return { x, x, x, }; }
MGM_INLINE float3 makef3(float x, float y, float z)
{ return { x, y, z }; }
MGM_INLINE float3 makef3_f2xy(float2 xy, float z)
{ return { xy.x, xy.y, z }; }
MGM_INLINE float3 makef3_f2yz(float x, float2 yz)
{ return { x, yz.x, yz.y }; }

#ifdef __cplusplus
MGM_INLINE float3 makef3(float x) { return make1f3(x); }
MGM_INLINE float3 makef3(float2 xy, float z) { return makef3_f2xy(xy, z); }
MGM_INLINE float3 makef3(float x, float2 yz) { return makef3_f2yz(x, yz); }
#endif


MGM_INLINE float4 make1f4(float x) { return { x, x, x, x }; }
MGM_INLINE float4 makef4(float x, float y, float z, float w)
{ return { x, y, z, w }; }
MGM_INLINE float4 makef4_f2xyzw(float2 xy, float2 zw)
{ return { xy.x, xy.y, zw.x, zw.y }; }
MGM_INLINE float4 makef4_f2xy(float2 xy, float z, float w)
{ return { xy.x, xy.y, z, w }; }
MGM_INLINE float4 makef4_f2yz(float x, float2 yz, float w)
{ return { x, yz.x, yz.y, w }; }
MGM_INLINE float4 makef4_f2zw(float x, float y, float2 zw)
{ return { x, y, zw.x, zw.y }; }
MGM_INLINE float4 makef4_f3xyz(float3 xyz, float w)
{ return { xyz.x, xyz.y, xyz.z, w }; }
MGM_INLINE float4 makef4_f3yzw(float x, float3 yzw)
{ return { x, yzw.x, yzw.y, yzw.z }; }

#ifdef __cplusplus
MGM_INLINE float4 makef4(float x) { return make1f4(x); }
MGM_INLINE float4 makef4(float2 xy, float2 zw) { return makef4_f2xyzw(xy, zw); }
MGM_INLINE float4 makef4(float2 xy, float z, float w)
{ return makef4_f2xy(xy, z, w); }
MGM_INLINE float4 makef4(float x, float2 yz, float w)
{ return makef4_f2yz(x, yz, w); }
MGM_INLINE float4 makef4(float x, float y, float2 zw)
{ return makef4_f2zw(x, y, zw); }
MGM_INLINE float4 makef4(float3 xyz, float w) { return makef4_f3xyz(xyz, w); }
MGM_INLINE float4 makef4(float x, float3 yzw) { return makef4_f3yzw(x, yzw); }
#endif

MGM_INLINE quatf makeqf(float i, float j, float k, float r)
{ return { i, j, k, r }; }
MGM_INLINE quatf makeqf_f4(float4 f4)
{ return { f4.x, f4.y, f4.z, f4.w }; }
MGM_INLINE quatf makeqf_axang(float3 axis, float angle)
{
	float half_angle = angle * 0.5f;
	float sin_half_angle = sinf(half_angle);
	return makeqf(
		axis.x * sin_half_angle,
		axis.y * sin_half_angle,
		axis.z * sin_half_angle,
		cosf(half_angle));
}
#ifdef __cplusplus
MGM_INLINE quatf makeqf(float4 f4) { return makeqf_f4(f4); }
MGM_INLINE quatf makeqf(float3 axis, float angle)
{ return makeqf_axang(axis, angle); }
#endif

MGM_INLINE float2x2 makef2x2_identity()
{ return { 1.0f, 0.0f, 0.0f, 1.0f }; }
MGM_INLINE float2x2 makef2x2(float xx, float xy, float yx, float yy)
{ return { xx, xy, yx, yy }; }
MGM_INLINE float2x2 makef2x2_f2(float2 x, float2 y)
{ return { x.x, x.y, y.x, y.y }; }

MGM_INLINE float3x3 makef3x3_identity()
{ return { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
MGM_INLINE float3x3 makef3x3(float xx, float xy, float xz,
														 float yx, float yy, float yz,
														 float zx, float zy, float zz)
{ return { xx, xy, xz, yx, yy, yz, zx, zy, zz }; }
MGM_INLINE float3x3 makef3x3_f3(float3 x, float3 y, float3 z)
{ return { x.x, x.y, x.z, y.x, y.y, y.z, z.x, z.y, z.z }; }

MGM_INLINE float4x4 makef4x4_identity()
{
	return {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
MGM_INLINE float4x4 makef4x4(
	float xx, float xy, float xz, float xw,
	float yx, float yy, float yz, float yw,
	float zx, float zy, float zz, float zw,
	float wx, float wy, float wz, float ww)
{
	return {
		xx, xy, xz, xw,
		yx, yy, yz, yw,
		zx, zy, zz, zw,
		wx, wy, wz, ww
	};
}
MGM_INLINE float4x4 makef4x4_f4(float4 x, float4 y, float4 z, float4 w)
{
	return {
		x.x, x.y, x.z, x.w,
		y.x, y.y, y.z, y.w,
		z.x, z.y, z.z, z.w,
		w.x, w.y, w.z, w.w
	};
}

#ifdef __cplusplus
MGM_INLINE float2x2 makef2x2(float2 x, float2 y) { return makef2x2_f2(x, y); }
MGM_INLINE float3x3 makef3x3(float3 x, float3 y, float3 z)
{ return makef3x3_f3(x, y, z); }
MGM_INLINE float4x4 makef4x4(float4 x, float4 y, float4 z, float4 w)
{ return makef4x4_f4(x, y, z, w); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Saturate
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float saturatef(float x) { return max(min(x, 1.0f), 0.0f); }
MGM_INLINE float2 saturatef2(float2 f2)
{ return makef2(saturatef(f2.x), saturatef(f2.y)); }
MGM_INLINE float3 saturatef3(float3 f3)
{ return makef3(saturatef(f3.x), saturatef(f3.y), saturatef(f3.z)); }
MGM_INLINE float4 saturatef4(float4 f4)
{
	return makef4(
		saturatef(f4.x),
		saturatef(f4.y),
		saturatef(f4.z),
		saturatef(f4.w));
}
#ifdef __cplusplus
MGM_INLINE float saturate(float x) { return saturatef(x); }
MGM_INLINE float2 saturate(float2 x) { return saturatef2(x); }
MGM_INLINE float3 saturate(float3 x) { return saturatef3(x); }
MGM_INLINE float4 saturate(float4 x) { return saturatef4(x); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Clamp
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float clampf(float x, float minv, float maxv)
{ return max(min(x, maxv), minv); }
MGM_INLINE float2 clampf2(float2 f2, float2 minv, float2 maxv)
{ return makef2(clampf(f2.x, minv.x, maxv.x), clampf(f2.y, minv.y, maxv.y)); }
MGM_INLINE float3 clampf3(float3 f3, float3 minv, float3 maxv)
{
	return makef3(
		clampf(f3.x, minv.x, maxv.x),
		clampf(f3.y, minv.y, maxv.y),
		clampf(f3.z, minv.z, maxv.z));
}
MGM_INLINE float4 clampf4(float4 f4, float4 minv, float4 maxv)
{
	return makef4(
		clampf(f4.x, minv.x, maxv.x),
		clampf(f4.y, minv.y, maxv.y),
		clampf(f4.z, minv.z, maxv.z),
		clampf(f4.w, minv.w, maxv.w));
}
#ifdef __cplusplus
MGM_INLINE float  clamp(float  x, float  minv, float  maxv)
{ return clampf(x, minv, maxv); }
MGM_INLINE float2 clamp(float2 x, float2 minv, float2 maxv)
{ return clampf2(x, minv, maxv); }
MGM_INLINE float3 clamp(float3 x, float3 minv, float3 maxv)
{ return clampf3(x, minv, maxv); }
MGM_INLINE float4 clamp(float4 x, float4 minv, float4 maxv)
{ return clampf4(x, minv, maxv); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Min
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 minf2(float2 a, float2 b)
{ return makef2(min(a.x, b.x), min(a.y, b.y)); }
MGM_INLINE float3 minf3(float3 a, float3 b)
{ return makef3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
MGM_INLINE float4 minf4(float4 a, float4 b)
{ return makef4(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w)); }
#ifdef __cplusplus
MGM_INLINE float2 minf(float2 a, float2 b) { return minf2(a, b); }
MGM_INLINE float3 minf(float3 a, float3 b) { return minf3(a, b); }
MGM_INLINE float4 minf(float4 a, float4 b) { return minf4(a, b); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Max
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 maxf2(float2 a, float2 b)
{ return makef2(max(a.x, b.x), max(a.y, b.y)); }
MGM_INLINE float3 maxf3(float3 a, float3 b)
{ return makef3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
MGM_INLINE float4 maxf4(float4 a, float4 b)
{ return makef4(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w)); }
#ifdef __cplusplus
MGM_INLINE float2 maxf(float2 a, float2 b) { return maxf2(a, b); }
MGM_INLINE float3 maxf(float3 a, float3 b) { return maxf3(a, b); }
MGM_INLINE float4 maxf(float4 a, float4 b) { return maxf4(a, b); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Abs
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 absf2(float2 a)
{ return makef2(fabsf(a.x), fabsf(a.y)); }
MGM_INLINE float3 absf3(float3 a)
{ return makef3(fabsf(a.x), fabsf(a.y), fabsf(a.z)); }
MGM_INLINE float4 absf4(float4 a)
{ return makef4(fabsf(a.x), fabsf(a.y), fabsf(a.z), fabsf(a.w)); }
#ifdef __cplusplus
MGM_INLINE float2 abs(float2 a) { return absf2(a); }
MGM_INLINE float3 abs(float3 a) { return absf3(a); }
MGM_INLINE float4 abs(float4 a) { return absf4(a); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Shuffle
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 shufflef2(float2 f2, int shuffle)
{ return makef2(f2.v[(shuffle) & 0x1], f2.v[(shuffle >> 1) & 0x1]); }
MGM_INLINE float3 shufflef3(float3 f3, int shuffle)
{
	return makef3(
		f3.v[(shuffle) & 0x3],
		f3.v[(shuffle >> 2) & 0x3],
		f3.v[(shuffle >> 4) & 0x3]);
}
MGM_INLINE float4 shufflef4(float4 f4, int shuffle)
{
	return makef4(
		f4.v[(shuffle) & 0x3],
		f4.v[(shuffle >> 2) & 0x3],
		f4.v[(shuffle >> 4) & 0x3],
		f4.v[(shuffle >> 6) & 0x3]);
}

////////////////////////////////////////////////////////////////////////////////
//Cross product
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float3 cross(float3 a, float3 b)
{
	return makef3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

////////////////////////////////////////////////////////////////////////////////
//Addition
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 addf2(float2 a, float2 b)
{ return makef2(a.x + b.x, a.y + b.y); }
MGM_INLINE float3 addf3(float3 a, float3 b)
{ return makef3(a.x + b.x, a.y + b.y, a.z + b.z); }
MGM_INLINE float4 addf4(float4 a, float4 b)
{ return makef4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
#ifdef __cplusplus
MGM_INLINE float2 add(float2 a, float2 b) { return addf2(a, b); }
MGM_INLINE float3 add(float3 a, float3 b) { return addf3(a, b); }
MGM_INLINE float4 add(float4 a, float4 b) { return addf4(a, b); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Subtraction
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 subf2(float2 a, float2 b)
{ return makef2(a.x - b.x, a.y - b.y); }
MGM_INLINE float3 subf3(float3 a, float3 b)
{ return makef3(a.x - b.x, a.y - b.y, a.z - b.z); }
MGM_INLINE float4 subf4(float4 a, float4 b)
{ return makef4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
#ifdef __cplusplus
MGM_INLINE float2 sub(float2 a, float2 b) { return subf2(a, b); }
MGM_INLINE float3 sub(float3 a, float3 b) { return subf3(a, b); }
MGM_INLINE float4 sub(float4 a, float4 b) { return subf4(a, b); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Negation
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 negf2(float2 a) { return makef2(-a.x, -a.y); }
MGM_INLINE float3 negf3(float3 a) { return makef3(-a.x, -a.y, -a.z); }
MGM_INLINE float4 negf4(float4 a) { return makef4(-a.x, -a.y, -a.z, -a.w); }
#ifdef __cplusplus
MGM_INLINE float2 neg(float2 a) { return negf2(a); }
MGM_INLINE float3 neg(float3 a) { return negf3(a); }
MGM_INLINE float4 neg(float4 a) { return negf4(a); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Multiplication
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 mulf2(float2 a, float2 b)
{ return makef2(a.x * b.x, a.y * b.y); }
MGM_INLINE float2 mulf2s(float2 a, float b)
{ return makef2(a.x * b, a.y * b); }
MGM_INLINE float2 mulsf2(float a, float2 b)
{ return makef2(a * b.x, a * b.y); }
MGM_INLINE float3 mulf3(float3 a, float3 b)
{ return makef3(a.x * b.x, a.y * b.y, a.z * b.z); }
MGM_INLINE float3 mulf3s(float3 a, float b)
{ return makef3(a.x * b, a.y * b, a.z * b); }
MGM_INLINE float3 mulsf3(float a, float3 b)
{ return makef3(a * b.x, a * b.y, a * b.z); }
MGM_INLINE float4 mulf4(float4 a, float4 b)
{ return makef4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
MGM_INLINE float4 mulf4s(float4 a, float b)
{ return makef4(a.x * b, a.y * b, a.z * b, a.w * b); }
MGM_INLINE float4 mulsf4(float a, float4 b)
{ return makef4(a * b.x, a * b.y, a * b.z, a * b.w); }
#ifdef __cplusplus
MGM_INLINE float2 mul(float2 a, float2 b) { return mulf2(a, b); }
MGM_INLINE float2 mul(float  a, float2 b) { return mulsf2(a, b); }
MGM_INLINE float2 mul(float2 a, float  b) { return mulf2s(a, b); }
MGM_INLINE float3 mul(float3 a, float3 b) { return mulf3(a, b); }
MGM_INLINE float3 mul(float  a, float3 b) { return mulsf3(a, b); }
MGM_INLINE float3 mul(float3 a, float  b) { return mulf3s(a, b); }
MGM_INLINE float4 mul(float4 a, float4 b) { return mulf4(a, b); }
MGM_INLINE float4 mul(float  a, float4 b) { return mulsf4(a, b); }
MGM_INLINE float4 mul(float4 a, float  b) { return mulf4s(a, b); }
#endif


////////////////////////////////////////////////////////////////////////////////
//Quaternion Multiplication
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE quatf mulqf(quatf a, quatf b)
{
	return makeqf(
		a.w * b.w - a.x * b.x + a.y * b.y - a.z * b.z,
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
		a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x);
}
MGM_INLINE float3 mulqf_f3(quatf q, float3 v)
{
	const float3 qv = makef3(q.i, q.j, q.k);
	const float3 uv = cross(qv, v);
	const float3 uuv = cross(qv, uv);
	return addf3(v, mulf3s(addf3(mulf3s(uv, q.r), uuv), 2));
}

////////////////////////////////////////////////////////////////////////////////
//Division
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 divf2(float2 a, float2 b)
{ return makef2(a.x / b.x, a.y / b.y); }
MGM_INLINE float2 divf2s(float2 a, float b)
{ return makef2(a.x / b, a.y / b); }
MGM_INLINE float2 divsf2(float a, float2 b)
{ return makef2(a / b.x, a / b.y); }
MGM_INLINE float3 divf3(float3 a, float3 b)
{ return makef3(a.x / b.x, a.y / b.y, a.z / b.z); }
MGM_INLINE float3 divf3s(float3 a, float b)
{ return makef3(a.x / b, a.y / b, a.z / b); }
MGM_INLINE float3 divsf3(float a, float3 b)
{ return makef3(a / b.x, a / b.y, a / b.z); }
MGM_INLINE float4 divf4(float4 a, float4 b)
{ return makef4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
MGM_INLINE float4 divf4s(float4 a, float b)
{ return makef4(a.x / b, a.y / b, a.z / b, a.w / b); }
MGM_INLINE float4 divsf4(float a, float4 b)
{ return makef4(a / b.x, a / b.y, a / b.z, a / b.w); }
#ifdef __cplusplus
MGM_INLINE float2 div(float2 a, float2 b) { return divf2(a, b); }
MGM_INLINE float2 div(float  a, float2 b) { return divsf2(a, b); }
MGM_INLINE float2 div(float2 a, float  b) { return divf2s(a, b); }
MGM_INLINE float3 div(float3 a, float3 b) { return divf3(a, b); }
MGM_INLINE float3 div(float  a, float3 b) { return divsf3(a, b); }
MGM_INLINE float3 div(float3 a, float  b) { return divf3s(a, b); }
MGM_INLINE float4 div(float4 a, float4 b) { return divf4(a, b); }
MGM_INLINE float4 div(float  a, float4 b) { return divsf4(a, b); }
MGM_INLINE float4 div(float4 a, float  b) { return divf4s(a, b); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Madd
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 maddf2(float2 a, float2 b, float2 c)
{ return addf2(mulf2(a, b), c);}
MGM_INLINE float3 maddf3(float3 a, float3 b, float3 c)
{ return addf3(mulf3(a, b), c);}
MGM_INLINE float4 maddf4(float4 a, float4 b, float4 c)
{ return addf4(mulf4(a, b), c);}
#ifdef __cplusplus
MGM_INLINE float2 madd(float2 a, float2 b, float2 c) { return maddf2(a, b, c); };
MGM_INLINE float3 madd(float3 a, float3 b, float3 c) { return maddf3(a, b, c); };
MGM_INLINE float4 madd(float4 a, float4 b, float4 c) { return maddf4(a, b, c); };
#endif

////////////////////////////////////////////////////////////////////////////////
//Dot product
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float dotf2(float2 a, float2 b)
{ return a.x * b.x + a.y * b.y; }
MGM_INLINE float dotf3(float3 a, float3 b)
{ return a.x * b.x + a.y * b.y + a.z * b.z; }
MGM_INLINE float dotf4(float4 a, float4 b)
{ return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
#ifdef __cplusplus
MGM_INLINE float dot(float2 a, float2 b) { return dotf2(a, b); };
MGM_INLINE float dot(float3 a, float3 b) { return dotf3(a, b); };
MGM_INLINE float dot(float4 a, float4 b) { return dotf4(a, b); };
#endif



////////////////////////////////////////////////////////////////////////////////
//Length
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float lengthf2(float2 a) { return sqrtf(dotf2(a, a)); }
MGM_INLINE float lengthf3(float3 a) { return sqrtf(dotf3(a, a)); }
MGM_INLINE float lengthf4(float4 a) { return sqrtf(dotf4(a, a)); }
#ifdef __cplusplus
MGM_INLINE float length(float2 a) { return lengthf2(a); };
MGM_INLINE float length(float3 a) { return lengthf3(a); };
MGM_INLINE float length(float4 a) { return lengthf4(a); };
#endif


////////////////////////////////////////////////////////////////////////////////
//Normalize
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 normalizef2(float2 a) { return divf2s(a, lengthf2(a)); }
MGM_INLINE float3 normalizef3(float3 a) { return divf3s(a, lengthf3(a)); }
MGM_INLINE float4 normalizef4(float4 a) { return divf4s(a, lengthf4(a)); }
#ifdef __cplusplus
MGM_INLINE float2 normalize(float2 a) { return normalizef2(a); };
MGM_INLINE float3 normalize(float3 a) { return normalizef3(a); };
MGM_INLINE float4 normalize(float4 a) { return normalizef4(a); };
#endif


////////////////////////////////////////////////////////////////////////////////
//Sum
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float sumf2(float2 a) { return a.x + a.y; }
MGM_INLINE float sumf3(float3 a) { return a.x + a.y + a.z; }
MGM_INLINE float sumf4(float4 a) { return a.x + a.y + a.z + a.w; }
#ifdef __cplusplus
MGM_INLINE float sum(float2 a) { return sumf2(a); }
MGM_INLINE float sum(float3 a) { return sumf3(a); }
MGM_INLINE float sum(float4 a) { return sumf4(a); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Matrix Transpose
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2x2 transposef2x2(float2x2 m)
{
	return makef2x2(m.v[0][0], m.v[1][0], m.v[0][1], m.v[1][1]);
}
MGM_INLINE float3x3 transposef3x3(float3x3 m)
{
	return makef3x3(
		m.v[0][0], m.v[1][0], m.v[2][0],
		m.v[0][1], m.v[1][1], m.v[2][1],
		m.v[0][2], m.v[1][2], m.v[2][2]);
}
MGM_INLINE float4x4 transposef4x4(float4x4 m)
{
	return makef4x4(
		m.v[0][0], m.v[1][0], m.v[2][0], m.v[3][0],
		m.v[0][1], m.v[1][1], m.v[2][1], m.v[3][1],
		m.v[0][2], m.v[1][2], m.v[2][2], m.v[3][2],
		m.v[0][3], m.v[1][3], m.v[2][3], m.v[3][3]);
}
#ifdef __cplusplus
MGM_INLINE float2x2 transpose(float2x2 m) { return transposef2x2(m); }
MGM_INLINE float3x3 transpose(float3x3 m) { return transposef3x3(m); }
MGM_INLINE float4x4 transpose(float4x4 m) { return transposef4x4(m); }
#endif



////////////////////////////////////////////////////////////////////////////////
//Multiply matrix with scalar
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2x2 mulf2x2s(float2x2 m, float s)
{ return makef2x2_f2(mulf2s(m.col[0], s), mulf2s(m.col[1], s)); }
MGM_INLINE float3x3 mulf3x3s(float3x3 m, float s)
{
	return makef3x3_f3(
		mulf3s(m.col[0], s),
		mulf3s(m.col[1], s),
		mulf3s(m.col[2], s));
}
MGM_INLINE float4x4 mulf4x4s(float4x4 m, float s)
{
	return makef4x4_f4(
		mulf4s(m.col[0], s),
		mulf4s(m.col[1], s),
		mulf4s(m.col[2], s),
		mulf4s(m.col[3], s));
}
#ifdef __cplusplus
MGM_INLINE float2x2 mul(float2x2 m, float s) { return mulf2x2s(m, s); }
MGM_INLINE float3x3 mul(float3x3 m, float s) { return mulf3x3s(m, s); }
MGM_INLINE float4x4 mul(float4x4 m, float s) { return mulf4x4s(m, s); }
#endif


////////////////////////////////////////////////////////////////////////////////
//Multiply matrix with vector
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2 mulf2x2_f2(float2x2 m, float2 v)
{
	return makef2(
		m.x.x * v.x + m.y.x * v.y,
		m.x.y * v.x + m.y.y * v.y);
}
MGM_INLINE float3 mulf3x3_f3(float3x3 m, float3 v)
{
	return makef3(
		m.x.x * v.x + m.y.x * v.y + m.z.x * v.z,
		m.x.y * v.x + m.y.y * v.y + m.z.y * v.z,
		m.x.z * v.x + m.y.z * v.y + m.z.z * v.z);
}
MGM_INLINE float4 mulf4x4_f4(float4x4 m, float4 v)
{
	return makef4(
		m.x.x * v.x + m.y.x * v.y + m.z.x * v.z + m.w.x * v.w,
		m.x.y * v.x + m.y.y * v.y + m.z.y * v.z + m.w.y * v.w,
		m.x.z * v.x + m.y.z * v.y + m.z.z * v.z + m.w.z * v.w,
		m.x.w * v.x + m.y.w * v.y + m.z.w * v.z + m.w.w * v.w);
}
#ifdef __cplusplus
MGM_INLINE float2 mul(float2x2 m, float2 v) { return mulf2x2_f2(m, v); }
MGM_INLINE float3 mul(float3x3 m, float3 v) { return mulf3x3_f3(m, v); }
MGM_INLINE float4 mul(float4x4 m, float4 v) { return mulf4x4_f4(m, v); }
#endif

////////////////////////////////////////////////////////////////////////////////
//Multiply matrix with matrix
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2x2 mulf2x2(float2x2 a, float2x2 b)
{
	return makef2x2_f2(
		addf2(mulf2s(a.col[0], b.v[0][0]), mulf2s(a.col[1], b.v[0][1])),
		addf2(mulf2s(a.col[0], b.v[1][0]), mulf2s(a.col[1], b.v[1][1])));
}
MGM_INLINE float3x3 mulf3x3(float3x3 a, float3x3 b)
{
	return makef3x3_f3(
		addf3(addf3(mulf3s(a.col[0], b.v[0][0]), mulf3s(a.col[1], b.v[0][1])),
					mulf3s(a.col[2], b.v[0][2])),
		addf3(addf3(mulf3s(a.col[0], b.v[1][0]), mulf3s(a.col[1], b.v[1][1])),
					mulf3s(a.col[2], b.v[1][2])),
		addf3(addf3(mulf3s(a.col[0], b.v[2][0]), mulf3s(a.col[1], b.v[2][1])),
					mulf3s(a.col[2], b.v[2][2])));
}
MGM_INLINE float4x4 mulf4x4(float4x4 a, float4x4 b)
{
#if 0
	float4 srca0 = a.col[0];
	float4 srca1 = a.col[1];
	float4 srca2 = a.col[2];
	float4 srca3 = a.col[3];

	float4 srcb0 = b.col[0];
	float4 srcb1 = b.col[1];
	float4 srcb2 = b.col[2];
	float4 srcb3 = b.col[3];

	float4x4 result;
	result.col[0] =
		addf4(addf4(mulf4s(srca0, srcb0[0]), mulf4s(srca1, srcb0[1])), 
					addf4(mulf4s(srca2, srcb0[2]), mulf4s(srca3, srcb0[3])));
	result.col[1] = addf4(addf4(mulf4s(srca0, srcb1[0]), mulf4s(srca1, srcb1[1])), addf4(mulf4s(srca2, srcb1[2]), mulf4s(srca3, srcb1[3])));
	result.col[2] = addf4(addf4(mulf4s(srca0, srcb2[0]), mulf4s(srca1, srcb2[1])), addf4(mulf4s(srca2, srcb2[2]), mulf4s(srca3, srcb2[3])));
	result.col[3] = addf4(addf4(mulf4s(srca0, srcb3[0]), mulf4s(srca1, srcb3[1])), addf4(mulf4s(srca2, srcb3[2]), mulf4s(srca3, srcb3[3])));
	return result;
#endif
	return makef4x4_f4(
		addf4(addf4(mulf4s(a.col[0], b.v[0][0]), mulf4s(a.col[1], b.v[0][1])),
					addf4(mulf4s(a.col[2], b.v[0][2]), mulf4s(a.col[3], b.v[0][3]))),
		addf4(addf4(mulf4s(a.col[0], b.v[1][0]), mulf4s(a.col[1], b.v[1][1])),
					addf4(mulf4s(a.col[2], b.v[1][2]), mulf4s(a.col[3], b.v[1][3]))),
		addf4(addf4(mulf4s(a.col[0], b.v[2][0]), mulf4s(a.col[1], b.v[2][1])),
					addf4(mulf4s(a.col[2], b.v[2][2]), mulf4s(a.col[3], b.v[2][3]))),
		addf4(addf4(mulf4s(a.col[0], b.v[3][0]), mulf4s(a.col[1], b.v[3][1])),
					addf4(mulf4s(a.col[2], b.v[3][2]), mulf4s(a.col[3], b.v[3][3]))));
}

#ifdef __cplusplus
MGM_INLINE float2x2 mul(float2x2 a, float2x2 b) { return mulf2x2(a, b); }
MGM_INLINE float3x3 mul(float3x3 a, float3x3 b) { return mulf3x3(a, b); }
MGM_INLINE float4x4 mul(float4x4 a, float4x4 b) { return mulf4x4(a, b); }
#endif


////////////////////////////////////////////////////////////////////////////////
//Projection matrix
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float4x4 orthoLHf4x4() { /* TODO */ return makef4x4_identity(); };
MGM_INLINE float4x4 orthoRHf4x4() { /* TODO */ return makef4x4_identity(); };
MGM_INLINE float4x4 perspectiveRHf4x4(
	float fov,
	float aspect,
	float znear,
	float zfar)
{
	float a = tanf(fov / 2.0f);
#ifdef MGM_Z_ZERO_TO_ONE
	return makef4x4_f4(
		makef4(1.0f / (aspect * a), 0.0f, 0.0f, 0.0f),
		makef4(0.0f, 1.0f / a, 0.0f, 0.0f),
		makef4(0.0f, 0.0f, zfar / (znear - zfar), -1.0f),
		makef4(0.0f, 0.0f, -(zfar * znear) / (zfar - znear), 0.0f));
#else
	return makef4x4_f4(
		makef4(1.0f / (aspect * a), 0.0f, 0.0f, 0.0f),
		makef4(0.0f, 1.0f / a, 0.0f, 0.0f),
		makef4(0.0f, 0.0f, -(zfar + znear) / (zfar - znear), -1.0f),
		makef4(0.0f, 0.0f, -(2.0f * zfar * znear) / (zfar - znear), 0.0f));
#endif
};
MGM_INLINE float4x4 perspectiveLHf4x4(
	float fov,
	float aspect,
	float znear,
	float zfar)
{
	float a = tanf(fov / 2.0f);
#ifdef MGM_Z_ZERO_TO_ONE
	return makef4x4_f4(
		makef4(1.0f / (aspect * a), 0.0f, 0.0f, 0.0f),
		makef4(0.0f, 1.0f / a, 0.0f, 0.0f),
		makef4(0.0f, 0.0f, zfar / (zfar - znear), 1.0f),
		makef4(0.0f, 0.0f, -(zfar * znear) / (zfar - znear), 0.0f));
#else
	return makef4x4_f4(
		makef4(1.0f / (aspect * a), 0.0f, 0.0f, 0.0f),
		makef4(0.0f, 1.0f / a, 0.0f, 0.0f),
		makef4(0.0f, 0.0f, (zfar + znear) / (zfar - znear), 1.0f),
		makef4(0.0f, 0.0f, -(2.0f * zfar * znear) / (zfar - znear), 0.0f));
#endif
};


MGM_INLINE float4x4 translatef4x4(float3 pos)
{
	return makef4x4_f4(
		makef4(1, 0, 0, 0),
		makef4(0, 1, 0, 0),
		makef4(0, 0, 1, 0),
		makef4_f3xyz(pos,  1.0f));
}
MGM_INLINE float4x4 scalef4x4(float3 scale)
{
	return makef4x4_f4(
		makef4(scale.x, 0, 0, 0),
		makef4(0, scale.y, 0, 0),
		makef4(0, 0, scale.z, 0),
		makef4(0, 0, 0, 1));
}
MGM_INLINE float4x4 rotatexf4x4(float r)
{
	float c = cosf(r);
	float s = sinf(r);
	return makef4x4_f4(
		makef4(1, 0, 0, 0),
		makef4(0, c,-s, 0),
		makef4(0, s, c, 0),
		makef4(0, 0, 0, 1));
}
MGM_INLINE float4x4 rotateyf4x4(float r)
{
	float c = cosf(r);
	float s = sinf(r);
	return makef4x4_f4(
		makef4( c, 0, s, 0),
		makef4( 0, 1, 0, 0),
		makef4(-s, 0, c, 0),
		makef4( 0, 0, 0, 1));
}
MGM_INLINE float4x4 rotatezf4x4(float r)
{
	float c = cosf(r);
	float s = sinf(r);
	return makef4x4_f4(
		makef4(c,-s, 0, 0),
		makef4(s, c, 0, 0),
		makef4(0, 0, 1, 0),
		makef4(0, 0, 0, 1));
}






////////////////////////////////////////////////////////////////////////////////
//Matrix Determinant
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float determinantf2x2(float2x2 m)
{
	return m.v[0][0] * m.v[1][1] - m.v[1][0] * m.v[0][1];
}

MGM_INLINE float determinantf3x3(float3x3 m)
{
	const float a = m.v[0][0] * (m.v[1][1] * m.v[2][2] - m.v[1][2] * m.v[2][1]);
	const float b = m.v[1][0] * (m.v[0][1] * m.v[2][2] - m.v[0][2] * m.v[2][1]);
	const float c = m.v[2][0] * (m.v[0][1] * m.v[1][2] - m.v[0][2] * m.v[1][1]);
	return a - b + c;
}

MGM_INLINE float determinantf4x4(float4x4 m)
{
	const float a = m.v[0][0] * (
		m.v[1][1] * (m.v[2][2] * m.v[3][3] - m.v[2][3] * m.v[3][2]) -
		m.v[2][1] * (m.v[1][2] * m.v[3][3] - m.v[1][3] * m.v[3][2]) +
		m.v[3][1] * (m.v[1][2] * m.v[2][3] - m.v[1][3] * m.v[2][2]));

	const float b = m.v[1][0] * (
		m.v[0][1] * (m.v[2][2] * m.v[3][3] - m.v[2][3] * m.v[3][2]) -
		m.v[2][1] * (m.v[0][2] * m.v[3][3] - m.v[0][3] * m.v[3][2]) +
		m.v[3][1] * (m.v[0][2] * m.v[2][3] - m.v[0][3] * m.v[2][2]));

	const float c = m.v[2][0] * (
		m.v[0][1] * (m.v[1][2] * m.v[3][3] - m.v[1][3] * m.v[3][2]) -
		m.v[1][1] * (m.v[0][2] * m.v[3][3] - m.v[0][3] * m.v[3][2]) +
		m.v[3][1] * (m.v[0][2] * m.v[1][3] - m.v[0][3] * m.v[1][2]));

	const float d = m.v[3][0] * (
		m.v[0][1] * (m.v[1][2] * m.v[2][3] - m.v[1][3] * m.v[2][2]) -
		m.v[1][1] * (m.v[0][2] * m.v[2][3] - m.v[0][3] * m.v[2][2]) +
		m.v[2][1] * (m.v[0][2] * m.v[1][3] - m.v[0][3] * m.v[1][2]));

	return a - b + c - d;
}
#ifdef __cplusplus
MGM_INLINE float determinant(float2x2 m) { return determinantf2x2(m); }
MGM_INLINE float determinant(float3x3 m) { return determinantf3x3(m); }
MGM_INLINE float determinant(float4x4 m) { return determinantf4x4(m); }
#endif


////////////////////////////////////////////////////////////////////////////////
//Matrix Inverse
////////////////////////////////////////////////////////////////////////////////
MGM_INLINE float2x2 inversef2x2(float2x2 m)
{
	float2x2 result;
	const float rdet = 1.0f / determinantf2x2(m);
	
	result.v[0][0] = m.v[1][1] * rdet;
	result.v[0][1] = m.v[0][1] * rdet;
	result.v[1][0] = m.v[1][0] * rdet;
	result.v[1][1] = m.v[0][0] * rdet;

	return result;
}
MGM_INLINE float3x3 inversef3x3(float3x3 m)
{
	float3x3 result;
	const float rdet = 1.0f / determinantf3x3(m);
	
	result.v[0][0] = +(m.v[1][1] * m.v[2][2] - m.v[2][1] * m.v[1][2]) * rdet;
	result.v[0][1] = -(m.v[0][1] * m.v[2][2] - m.v[2][1] * m.v[0][2]) * rdet;
	result.v[0][2] = +(m.v[0][1] * m.v[1][2] - m.v[1][1] * m.v[0][2]) * rdet;
	result.v[1][0] = -(m.v[1][0] * m.v[2][2] - m.v[2][0] * m.v[1][2]) * rdet;
	result.v[1][1] = +(m.v[0][0] * m.v[2][2] - m.v[2][0] * m.v[0][2]) * rdet;
	result.v[1][2] = -(m.v[0][0] * m.v[1][2] - m.v[1][0] * m.v[0][2]) * rdet;
	result.v[2][0] = +(m.v[1][0] * m.v[2][1] - m.v[2][0] * m.v[1][1]) * rdet;
	result.v[2][1] = -(m.v[0][0] * m.v[2][1] - m.v[2][0] * m.v[0][1]) * rdet;
	result.v[2][2] = +(m.v[0][0] * m.v[1][1] - m.v[1][0] * m.v[0][1]) * rdet;

	return result;
}
//based off of SCE vector math vmathM4Inverse:
// https://github.com/erwincoumans/sce_vectormath/blob/master/include/vectormath/scalar/c/mat_aos.h
MGM_INLINE float4x4 inversef4x4(float4x4 ma)
{
	float4x4 result;

	const float a = ma.v[0][0], b = ma.v[0][1], c = ma.v[0][2], d = ma.v[0][3];
	const float e = ma.v[1][0], f = ma.v[1][1], g = ma.v[1][2], h = ma.v[1][3];
	const float i = ma.v[2][0], j = ma.v[2][1], k = ma.v[2][2], l = ma.v[2][3];
	const float m = ma.v[3][0], n = ma.v[3][1], o = ma.v[3][2], p = ma.v[3][3];

	float tmp0 = ((k * d) - (c * l));
	float tmp1 = ((o * h) - (g * p));
	float tmp2 = ((b * k) - (j * c));
	float tmp3 = ((f * o) - (n * g));
	float tmp4 = ((j * d) - (b * l));
	float tmp5 = ((n * h) - (f * p));

	const float4 res0 = makef4(
		(j * tmp1) - (l * tmp3) - (k * tmp5),
		(n * tmp0) - (p * tmp2) - (o * tmp4),
		(d * tmp3) + (c * tmp5) - (b * tmp1),
		(h * tmp2) + (g * tmp4) - (f * tmp0));

	const float det_inv = 1.0f / dotf4(res0, makef4(a, e, i, m));
	
	float4 res1 = makef4(i * tmp1, m * tmp0, a * tmp1, e * tmp0);
	float4 res3 = makef4(i * tmp3, m * tmp2, a * tmp3, e * tmp2);
	float4 res2 = makef4(i * tmp5, m * tmp4, a * tmp5, e * tmp4);

	tmp0 = (i * b) - (a * j);
	tmp1 = (m * f) - (e * n);
	tmp2 = (i * d) - (a * l);
	tmp3 = (m * h) - (e * p);
	tmp4 = (i * c) - (a * k);
	tmp5 = (m * g) - (e * o);

	res2 = makef4(
		(l * tmp1) - (j * tmp3) + res2.x,
		(p * tmp0) - (n * tmp2) + res2.y,
		(b * tmp3) - (d * tmp1) - res2.z,
		(f * tmp2) - (h * tmp0) - res2.w);

	res3 = makef4(
		(j * tmp5) - (k * tmp1) + res3.x,
		(n * tmp4) - (o * tmp0) + res3.y,
		(c * tmp1) - (b * tmp5) - res3.z,
		(g * tmp0) - (f * tmp4) - res3.w);

	res1 = makef4(
		(k * tmp3) - (l * tmp5) - res1.x,
		(o * tmp2) - (p * tmp4) - res1.y,
		(d * tmp5) - (c * tmp3) + res1.z,
		(h * tmp4) - (g * tmp2) + res1.w);

	result.x = mulf4s(res0, det_inv);
	result.y = mulf4s(res1, det_inv);
	result.z = mulf4s(res2, det_inv);
	result.w = mulf4s(res3, det_inv);

	return result;
}
#ifdef __cplusplus
MGM_INLINE float2x2 inverse(float2x2 m) { return inversef2x2(m); }
MGM_INLINE float3x3 inverse(float3x3 m) { return inversef3x3(m); }
MGM_INLINE float4x4 inverse(float4x4 m) { return inversef4x4(m); }
#endif



//Operator overloading
#ifdef __cplusplus
MGM_INLINE float2 operator+(float2 a, float2 b) { return addf2(a, b); }
MGM_INLINE float2 operator+(float2 a)           { return a; }
MGM_INLINE float2 operator-(float2 a, float2 b) { return subf2(a, b); }
MGM_INLINE float2 operator-(float2 a)           { return negf2(a); }
MGM_INLINE float2 operator*(float2 a, float2 b) { return mulf2(a, b); }
MGM_INLINE float2 operator*(float2 a, float  b) { return mulf2s(a, b); }
MGM_INLINE float2 operator*(float  a, float2 b) { return mulsf2(a, b); }
MGM_INLINE float2 operator/(float2 a, float2 b) { return divf2(a, b); }
MGM_INLINE float2 operator/(float2 a, float  b) { return divf2s(a, b); }
MGM_INLINE float2 operator/(float  a, float2 b) { return divsf2(a, b); }
MGM_INLINE float2 operator+=(float2& a, float2 b) { return a = a + b; };
MGM_INLINE float2 operator-=(float2& a, float2 b) { return a = a - b; };
MGM_INLINE float2 operator*=(float2& a, float2 b) { return a = a * b; };
MGM_INLINE float2 operator*=(float2& a, float  b) { return a = a * b; };
MGM_INLINE float2 operator/=(float2& a, float2 b) { return a = a / b; };
MGM_INLINE float2 operator/=(float2& a, float  b) { return a = a / b; };


MGM_INLINE float3 operator+(float3 a, float3 b) { return addf3(a, b); }
MGM_INLINE float3 operator+(float3 a)           { return a; }
MGM_INLINE float3 operator-(float3 a, float3 b) { return subf3(a, b); }
MGM_INLINE float3 operator-(float3 a)           { return negf3(a); }
MGM_INLINE float3 operator*(float3 a, float3 b) { return mulf3(a, b); }
MGM_INLINE float3 operator*(float3 a, float  b) { return mulf3s(a, b); }
MGM_INLINE float3 operator*(float  a, float3 b) { return mulsf3(a, b); }
MGM_INLINE float3 operator/(float3 a, float3 b) { return divf3(a, b); }
MGM_INLINE float3 operator/(float3 a, float  b) { return divf3s(a, b); }
MGM_INLINE float3 operator/(float  a, float3 b) { return divsf3(a, b); }
MGM_INLINE float3 operator+=(float3& a, float3 b) { return a = a + b; };
MGM_INLINE float3 operator-=(float3& a, float3 b) { return a = a - b; };
MGM_INLINE float3 operator*=(float3& a, float3 b) { return a = a * b; };
MGM_INLINE float3 operator*=(float3& a, float  b) { return a = a * b; };
MGM_INLINE float3 operator/=(float3& a, float3 b) { return a = a / b; };
MGM_INLINE float3 operator/=(float3& a, float  b) { return a = a / b; };


MGM_INLINE float4 operator+(float4 a, float4 b) { return addf4(a, b); }
MGM_INLINE float4 operator+(float4 a)           { return a; }
MGM_INLINE float4 operator-(float4 a, float4 b) { return subf4(a, b); }
MGM_INLINE float4 operator-(float4 a)           { return negf4(a); }
MGM_INLINE float4 operator*(float4 a, float4 b) { return mulf4(a, b); }
MGM_INLINE float4 operator*(float4 a, float  b) { return mulf4s(a, b); }
MGM_INLINE float4 operator*(float  a, float4 b) { return mulsf4(a, b); }
MGM_INLINE float4 operator/(float4 a, float4 b) { return divf4(a, b); }
MGM_INLINE float4 operator/(float4 a, float  b) { return divf4s(a, b); }
MGM_INLINE float4 operator/(float  a, float4 b) { return divsf4(a, b); }
MGM_INLINE float4 operator+=(float4& a, float4 b) { return a = a + b; };
MGM_INLINE float4 operator-=(float4& a, float4 b) { return a = a - b; };
MGM_INLINE float4 operator*=(float4& a, float4 b) { return a = a * b; };
MGM_INLINE float4 operator*=(float4& a, float  b) { return a = a * b; };
MGM_INLINE float4 operator/=(float4& a, float4 b) { return a = a / b; };
MGM_INLINE float4 operator/=(float4& a, float  b) { return a = a / b; };


MGM_INLINE float2x2 operator*(float2x2 a, float2x2 b) { return mulf2x2(a, b); }
MGM_INLINE float2   operator*(float2x2 a, float2   b) { return mulf2x2_f2(a, b); }
MGM_INLINE float2x2 operator*(float2x2 a, float    b) { return mulf2x2s(a, b); }
MGM_INLINE float3x3 operator*(float3x3 a, float3x3 b) { return mulf3x3(a, b); }
MGM_INLINE float3   operator*(float3x3 a, float3   b) { return mulf3x3_f3(a, b); }
MGM_INLINE float3x3 operator*(float3x3 a, float    b) { return mulf3x3s(a, b); }
MGM_INLINE float4x4 operator*(float4x4 a, float4x4 b) { return mulf4x4(a, b); }
MGM_INLINE float4   operator*(float4x4 a, float4   b) { return mulf4x4_f4(a, b); }
MGM_INLINE float4x4 operator*(float4x4 a, float    b) { return mulf4x4s(a, b); }

#endif


#if  defined(_MSVC)
#  pragma warning(pop)
#elif defined(__clang__)
#  pragma clang diagnostic pop
#endif

#endif //INCLUDED_MGM_H
