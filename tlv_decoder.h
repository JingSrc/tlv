#ifndef TLV_DECODER_H
#define TLV_DECODER_H

#include "tlv_global.h"

namespace tlv {

class Field;
class Struct;

class FieldEncoder {
public:
    tlv::byte *encode(const Field &field, tlv::byte *ptr, size_t len);
    tlv::State decode(Field &field, const void *ptr, size_t len);
};

class StrcutDecoder {
public:
    tlv::byte *encode(const Struct &stru, tlv::byte *ptr, int len);
    tlv::State decode(Struct &stru, const void *ptr, size_t len);
};


}

#endif // TLV_DECODER_H
