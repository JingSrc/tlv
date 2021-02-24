#ifndef TLV_PRIVATE_H
#define TLV_PRIVATE_H

#include "tlv_global.h"

namespace tlv::internal {

struct FieldPrivate {
    tlv::byte tag;
    int       size;
    tlv::byte value[0];
} __attribute__((packed));

}

#endif // TLV_PRIVATE_H
