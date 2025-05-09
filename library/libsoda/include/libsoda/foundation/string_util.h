#ifndef _LIBSODA_FOUNDATION_STRING_UTIL_H_
#define _LIBSODA_FOUNDATION_STRING_UTIL_H_

#include "libsoda/foundation/string.h"

#include <vector>

_namespace_soda_util_begin

// about path

const String lastPathComponent(const String& path);

void removeLastPathComponent(String* pPath);

void removeLastPathSeparator(String* pPath);


// about file

const String filenameOnly(const String& path);

const String fileExtensionOnly(const String& path);

bool checkExtension(const String& path, const String& ext);


// about type

int32_t htoi(const String& hex);

void binToHex(String* pDst, const uint8_t* pSrc, const int32_t len);

void hexToBin(uint8_t* pDst, const String& src);


// about regex

const String numericSubStr(const String& str);

_namespace_soda_util_end


#define _namespace_soda_util_base64_begin _namespace_soda_util_begin namespace base64 {
#define _namespace_soda_util_base64_end   } _namespace_soda_util_end

_namespace_soda_util_base64_begin

void encode(uint8_t* pDst, const uint8_t* pSrc, size_t len);

void encode(Binary* pDest, const Binary* pSrc, const size_t len);

size_t decode(uint8_t* pDest, const uint8_t* pSrc);

size_t decode(Binary* pDest, const uint8_t* pSrc);

const String encodedString(const String& src);

const String decodedString(const String& src);

const String encodedBinary(Binary* pSrc);

BinaryPtr decodedBinary(const String* pStr);

_namespace_soda_util_base64_end

#endif	// _LIBSODA_FOUNDATION_STRING_UTIL_H_