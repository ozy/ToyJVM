#ifndef ENDIAN_H_
#define ENDIAN_H_

// endian.h was not cross platform and was under a different
// name in different OS'es. Assuming most of the desktop cpu's 
// are little endian, I forged these functions into internal swap
// functions in gcc and clang. The visual studio users may use
// the _byteswap_ushort, _byteswap_ulong, _byteswap_uint64 functions
// respectively. 
#define be16toh(x) __builtin_bswap16(x)
#define be32toh(x) __builtin_bswap32(x)
#define be64toh(x) __builtin_bswap64(x)

#endif