#include "libsoda/foundation/string_util.h"

#include <sstream>

_namespace_soda_util_begin

const String lastPathComponent(const String& path)
{
    size_t i = path.findLastOf(L"/\\");
    if (i == String::npos) {
        return path;
    }

    size_t length = path.length();
    if (length == 1) {
        return path;
    }
    else if (i < length - 1) {
        return path.substring(i + 1);
    }
    else {
        i = path.findLastOf(L"/\\", length - 2);
        return path.substring(i + 1, length - 2 - (i + 1) + 1);
    }
}


void removeLastPathComponent(String* pPath)
{
    removeLastPathSeparator(pPath);
    String s = lastPathComponent(*pPath);
    if (0 < s.length() && s != *pPath) {
        pPath->resize(pPath->length() - s.length());
        removeLastPathSeparator(pPath);
    }
}


void removeLastPathSeparator(String* pPath)
{
    size_t i = pPath->findLastOf(L"/\\");
    if (i != String::npos) {
        size_t length = pPath->length();
        if (0 < i && i == length - 1) {
            pPath->resize(length - 1);
        }
    }
}


const String filenameOnly(const String& path)
{
    size_t si = path.findLastOf(L"/\\");
    si = (si == String::npos) ? 0 : (si + 1);

    size_t ei = path.findLastOf(L".");
    if (ei == String::npos) {
        ei = path.length();
    }

    return (si == ei) ? path : path.substring(si, ei - si);
}


const String fileExtensionOnly(const String& path)
{
#if defined(__linux__)
    if (path == L"." || path == L"..") {
        return String(L"");
    }
#endif
    size_t i = path.findLastOf(L".");
    return (i == String::npos) ? String(L"") : path.substring(i + 1);
}


bool checkExtension(const String& path, const String& ext)
{
#if defined(_WIN64)
    String extension = fileExtensionOnly(path);
    return fileExtensionOnly(path).caseInsensitiveCompare(ext);
#else
    return fileExtensionOnly(path) == ext;
#endif
}


#if defined(__linux__)
int32_t htoi(const String& hex)
{
    int32_t temp = 0;
    std::istringstream in(hex.to_string());
    in >> std::hex >> temp;

    return temp;
}


inline int32_t sbcsCode(const char** s) // single-byte character set
{
    return int32_t(*(*s)++) & 0x000000ff;
}


inline int32_t mbcsCode(const char** s) // multi-byte character set
{
    int32_t code = int32_t(*(*s)++) & 0x000000ff;
    if ((code & 0x80) && **s != '\0') {
        code = (code << 8) | (int32_t(*(*s)++) & 0x000000ff);
    }
    return code;
}


inline bool isVisibleChar(const char ch)
{
    return (0x20 <= ch && 0x7F != ch);
}


uint32_t replaceIllegalUtf8Internal(const char* src, const char rep, bool isMBCS, char* cString, String* pStr)
{
    if (!src || '\0' == src[0]) {
        return 0;
    }

    if (' ' != rep && !::isalnum(rep)) {
        return 0;
    }

    if (!cString && !pStr) {
        return 0;
    }

    const char repStr[2] = { rep, 0 };
    const char* s = src;
    const char* p;
    char tmp[3];

    int32_t bytes = 0;

    while (s && '\0' != *s) {
        p = s;
        int32_t code = (isMBCS ? mbcsCode(&s) : sbcsCode(&s));
        if (code) {
            int32_t len = (int32_t)(s - p);
            if (1 == len) {
                tmp[0] = p[0];
                tmp[1] = '\0';
            }
            else if (2 == len) {
                tmp[0] = p[0];
                tmp[1] = p[1];
                tmp[2] = '\0';
            }
            else {
                //assert(false, L"invalid charcode!");
            }

            // detecting illegal UTF-8 character.
            //  - should system-locale properly set.
            String utf8Char(tmp);

            if (utf8Char.length() > 0 && (2 == len || isVisibleChar(tmp[0]))) {
                if (cString) {
                    // convetible to UTF-8 character.
                    ::strcat(cString, tmp);
                }
                else {
                    pStr->append(utf8Char);
                }
                bytes += len;
            }
            else {
                // un-convetible to UTF-8 character.
                if (cString) {
                    ::strcat(cString, repStr);
                }
                else {
                    pStr->appendf(L"%s", repStr);
                }
                bytes++;
                if (2 == len) {
                    tmp[0] = p[1];
                    tmp[1] = '\0';
                    String utf8Char2(tmp);
                    if (utf8Char2.length() > 0 && isVisibleChar(tmp[0])) {
                        if (cString) {
                            // convetible to UTF-8 character.
                            ::strcat(cString, tmp);
                        }
                        else {
                            pStr->append(utf8Char2);
                        }
                        bytes++;
                    }
                    else {
                        // un-convetible to UTF-8 character.
                        if (cString) {
                            ::strcat(cString, repStr);
                        }
                        else {
                            pStr->appendf(L"%s", repStr);
                        }
                        bytes++;
                    }
                }
            }
        }
    }

    return bytes;
}


uint32_t replaceIllegalUtf8(char* cString, const char* src, bool isMBCS, const char rep)
{
    return replaceIllegalUtf8Internal(src, rep, isMBCS, cString, 0);
}

uint32_t replaceIllegalUtf8(String* utf8String, const char* src, bool isMBCS, const char rep)
{
    return replaceIllegalUtf8Internal(src, rep, isMBCS, 0, utf8String);
}

#endif  // __linux__

void binToHex(String* pDst, const uint8_t* pSrc, const int32_t len)
{
    *pDst = L"";
    String temp;
    for (int32_t i = 0; i < len; ++i) {
        temp.format(L"%02X", pSrc[i]);
        pDst->append(temp);
    }
}


void hexToBin(uint8_t* pDst, const String& src)
{
    int32_t index = 0;
    for (size_t i = 0; i < src.length(); i = i + 2) {
        std::wstring upper = src.substring(i, 1).to_wstring();
        std::wstring lower = src.substring(i + 1, 1).to_wstring();

        pDst[index++] = static_cast<uint8_t>((wcstol(upper.c_str(), 0, 16) << 4) + wcstol(lower.c_str(), 0, 16));
    }
}


const String numericSubStr(const String& str)
{
    int32_t start = -1, end = -1;
    String ret;
    for (int32_t i = 0; i < static_cast<int32_t>(str.length()); ++i) {
        if (start < 0 && L'0' <= str[i] && str[i] <= L'9') {
            start = i;
        }
        else if (start >= 0 && (L'0' > str[i] || str[i] > L'9')) {
            end = i;
            break;
        }
    }

    if (start >= 0) {
        if (end < 0) {
            end = static_cast<int32_t>(str.length());
        }
        ret = str.substring(start, end - start);
    }
    
    return ret;
}

_namespace_soda_util_end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_namespace_soda_util_base64_begin

const char g_base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const int8_t BAD = -1;
const int8_t base64val[] = {
    BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD,
    BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD,
    BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD, 62, BAD,BAD,BAD, 63,
     52, 53, 54, 55,  56, 57, 58, 59,  60, 61,BAD,BAD, BAD,BAD,BAD,BAD,
    BAD,  0,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,
     15, 16, 17, 18,  19, 20, 21, 22,  23, 24, 25,BAD, BAD,BAD,BAD,BAD,
    BAD, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, 40,
     41, 42, 43, 44,  45, 46, 47, 48,  49, 50, 51,BAD, BAD,BAD,BAD,BAD
};

inline int8_t decode64(uint8_t c) { return isascii(c) ? base64val[c] : BAD; }


void encode(uint8_t* pDst, const uint8_t* pSrc, size_t len)
{
    for (; 3 <= len; len -= 3) {
        *pDst++ = g_base64digits[pSrc[0] >> 2];
        *pDst++ = g_base64digits[((pSrc[0] << 4) & 0x30) | (pSrc[1] >> 4)];
        *pDst++ = g_base64digits[((pSrc[1] << 2) & 0x3c) | (pSrc[2] >> 6)];
        *pDst++ = g_base64digits[pSrc[2] & 0x3f];
        pSrc += 3;
    }

    if (0 < len) {
        uint8_t fragment;
        *pDst++ = g_base64digits[pSrc[0] >> 2];
        fragment = (pSrc[0] << 4) & 0x30;
        if (1 < len) {
            fragment |= pSrc[1] >> 4;
        }
        *pDst++ = g_base64digits[fragment];
        *pDst++ = (len < 2) ? '=' : g_base64digits[(pSrc[1] << 2) & 0x3c];
        *pDst++ = '=';
    }

    *pDst = 0;
}


void encode(Binary* pDest, const Binary* pSrc, const size_t len)
{
    size_t index = 0;
    size_t srcLength = len;
    for (; 3 <= srcLength; srcLength -= 3) {
        pDest->push_back(g_base64digits[(*pSrc)[index + 0] >> 2]);
        pDest->push_back(g_base64digits[(((*pSrc)[index + 0] << 4) & 0x30) | ((*pSrc)[index + 1] >> 4)]);
        pDest->push_back(g_base64digits[(((*pSrc)[index + 1] << 2) & 0x3c) | ((*pSrc)[index + 2] >> 6)]);
        pDest->push_back(g_base64digits[(*pSrc)[index + 2] & 0x3f]);
        index += 3;
    }

    if (0 < srcLength) {
        uint8_t fragment;
        pDest->push_back(g_base64digits[(*pSrc)[index + 0] >> 2]);
        fragment = ((*pSrc)[index + 0] << 4) & 0x30;
        if (1 < srcLength) {
            fragment |= (*pSrc)[index + 1] >> 4;
        }
        pDest->push_back(g_base64digits[fragment]);
        pDest->push_back((srcLength < 2) ? '=' : g_base64digits[((*pSrc)[index + 1] << 2) & 0x3c]);
        pDest->push_back('=');
    }

    pDest->push_back(0);
}


size_t decode(uint8_t* pDest, const uint8_t* pSrc)
{
    size_t len = 0;
    register uint8_t digit1, digit2, digit3, digit4;

    if (pSrc[0] == '+' && pSrc[1] == ' ') {
        pSrc += 2;
    }
    if (*pSrc == '\r') {
        return 0;
    }

    do {
        digit1 = pSrc[0];
        if (decode64(digit1) == BAD) {
            return -1;
        }
        
        digit2 = pSrc[1];
        if (decode64(digit2) == BAD) {
            return -1;
        }
        
        digit3 = pSrc[2];
        if (digit3 != '=' && decode64(digit3) == BAD) {
            return -1;
        }
        
        digit4 = pSrc[3];
        if (digit4 != '=' && decode64(digit4) == BAD) {
            return -1;
        }

        pSrc += 4;
        *pDest++ = (decode64(digit1) << 2) | (decode64(digit2) >> 4);
        ++len;
        if (digit3 != '=') {
            *pDest++ = ((decode64(digit2) << 4) & 0xf0) | (decode64(digit3) >> 2);
            ++len;
            if (digit4 != '=') {
                *pDest++ = ((decode64(digit3) << 6) & 0xc0) | decode64(digit4);
                ++len;
            }
        }
    } while (*pSrc && *pSrc != '\r' && digit4 != '=');

    return len;
}


size_t decode(Binary* pDest, const uint8_t* pSrc)
{
    size_t len = 0;
    register uint8_t digit1, digit2, digit3, digit4;

    if (pSrc[0] == '+' && pSrc[1] == ' ') {
        pSrc += 2;
    }
    if (*pSrc == '\r') {
        return 0;
    }

    do {
        digit1 = pSrc[0];
        if (decode64(digit1) == BAD) {
            return -1;
        }
        digit2 = pSrc[1];
        if (decode64(digit2) == BAD) {
            return -1;
        }
        digit3 = pSrc[2];
        if (digit3 != '=' && decode64(digit3) == BAD) {
            return -1;
        }
        digit4 = pSrc[3];
        if (digit4 != '=' && decode64(digit4) == BAD) {
            return -1;
        }
        pSrc += 4;
        pDest->push_back((decode64(digit1) << 2) | (decode64(digit2) >> 4));
        ++len;
        if (digit3 != '=') {
            pDest->push_back(((decode64(digit2) << 4) & 0xf0) | (decode64(digit3) >> 2));
            ++len;
            if (digit4 != '=') {
                pDest->push_back(((decode64(digit3) << 6) & 0xc0) | decode64(digit4));
                ++len;
            }
        }
    } while (*pSrc && *pSrc != '\r' && digit4 != '=');

    return len;
}


const String encodedString(const String& src)
{
    size_t allocSize = static_cast<size_t>(src.length() * 1.5);
    uint8_t* pEnc = new uint8_t[allocSize];
    std::memset(pEnc, 0, allocSize);
    encode(pEnc, reinterpret_cast<const uint8_t*>(src.to_string().c_str()), src.length());
    String encodedStr(reinterpret_cast<char*>(pEnc));
    delete[] pEnc;

    return encodedStr;
}


const String decodedString(const String& src)
{
    size_t allocSize = src.length();
    uint8_t* pDst = new uint8_t[allocSize];
    std::memset(pDst, 0, allocSize);
    decode(pDst, reinterpret_cast<const uint8_t*>(src.to_string().c_str()));

    return String(reinterpret_cast<char*>(pDst));
}


const String encodedBinary(Binary* pSrc)
{
    Binary dst = {};
    encode(&dst, pSrc, pSrc->size());

    return String(std::string(dst.begin(), dst.end()));
}


BinaryPtr decodedbinary(const String* pStr)
{
    BinaryPtr pDest = std::make_shared<Binary>();
    decode(pDest.get(), reinterpret_cast<const uint8_t*>(pStr->to_string().c_str()));

    return pDest;
}

_namespace_soda_util_base64_end