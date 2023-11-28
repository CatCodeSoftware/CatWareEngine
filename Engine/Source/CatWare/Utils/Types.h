/**
* @file Types.h
* 
* @brief Contains some typedefs for diffrent datatypes.
* 
* @author PointThink
*/

#pragma once

#include <stdint.h>

// Byte
typedef unsigned char Byte;

// i don't really like the standard naming convention for the int types so i typedefed my own
// 8 bit
typedef int8_t Int8;
typedef uint8_t UInt8;

// 16 bit
typedef int16_t Int16;
typedef uint16_t UInt16;

// 32 bit
typedef int32_t Int32;
typedef uint32_t UInt32;

// 64 bit
typedef int64_t Int64;
typedef uint64_t UInt64;

// Boolean
typedef unsigned char Bool; // a boolean should only really take up one byte

#ifndef __cplusplus
#define false 0
#define true 1
#endif

// void pointer
typedef void* VoidPtr;
typedef void* Handle;

// String
typedef char* String;

#define NULL 0