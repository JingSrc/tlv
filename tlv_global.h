#ifndef TLV_GLOBAL_H
#define TLV_GLOBAL_H

#include <stdint.h>

#include <memory>

namespace tlv {

using byte = unsigned char;

enum class State {
    NotReady,
    Error,
    Success
};

template <typename T>
std::shared_ptr<T> allocate(size_t size) {
    return std::shared_ptr<T>((T *)::malloc(size), ::free);
}

template<typename T> T endian_swap(T s);

template <> inline uint64_t endian_swap<uint64_t>(uint64_t s)
{
    return 0 | ((s & 0x00000000000000ffULL) << 56) | ((s & 0x000000000000ff00ULL) << 40) | ((s & 0x0000000000ff0000ULL) << 24)
        | ((s & 0x00000000ff000000ULL) << 8) | ((s & 0x000000ff00000000ULL) >> 8) | ((s & 0x0000ff0000000000ULL) >> 24)
        | ((s & 0x00ff000000000000ULL) >> 40) | ((s & 0xff00000000000000ULL) >> 56);
}

template <> inline uint32_t endian_swap<uint32_t>(uint32_t s)
{
    return 0 | ((s & 0x000000ff) << 24) | ((s & 0x0000ff00) << 8) | ((s & 0x00ff0000) >> 8) | ((s & 0xff000000) >> 24);
}

template <> inline uint16_t endian_swap<uint16_t>(uint16_t s)
{
    return uint16_t(0 | ((s & 0x00ff) << 8) | ((s & 0xff00) >> 8));
}

template <> inline uint8_t endian_swap<uint8_t>(uint8_t s)
{
    return s;
}

template <> inline int64_t endian_swap<int64_t>(int64_t s)
{
    return endian_swap<uint64_t>(uint64_t(s));
}

template <> inline int32_t endian_swap<int32_t>(int32_t s)
{
    return endian_swap<uint32_t>(uint32_t(s));
}

template <> inline int16_t endian_swap<int16_t>(int16_t s)
{
    return endian_swap<uint16_t>(uint16_t(s));
}

template <> inline int8_t endian_swap<int8_t>(int8_t s)
{
    return endian_swap<uint8_t>(uint8_t(s));
}

}

#endif // TLV_GLOBAL_H
