#include "tlv_private.h"
#include "tlv_decoder.h"
#include "tlv_field.h"
#include "tlv_struct.h"

using namespace tlv;

tlv::byte *FieldEncoder::encode(const Field &field, byte *ptr, size_t len)
{
    size_t fieldLen = field.capacity();
    if (len < fieldLen) {
        return ptr;
    }

    auto tag = field.tag();
    auto size = endian_swap(field.size());
    auto value = field.value();

    std::memcpy(ptr, &tag, sizeof(tag));
    std::memcpy(ptr + sizeof(tag), &size, sizeof(size));
    std::memcpy(ptr + sizeof(tag) + sizeof(size), value, field.size());

    return ptr + fieldLen;
}

tlv::State FieldEncoder::decode(Field &field, const void *ptr, size_t len)
{
    if (len < sizeof(internal::FieldPrivate)) {
        return tlv::State::NotReady;
    }

    auto pField = reinterpret_cast<const internal::FieldPrivate *>(ptr);
    auto size = endian_swap(pField->size);
    if (size < 0) {
        return tlv::State::Error;
    }

    int dataSize = len - sizeof(internal::FieldPrivate);
    if (dataSize < size) {
        return tlv::State::NotReady;
    }

    field.reset(pField->tag, size, pField->value);

    return tlv::State::Success;
}

byte *StrcutDecoder::encode(const Struct &stru, tlv::byte *ptr, int len)
{
    FieldEncoder encoder;

    auto temp = ptr;
    int offset = 0;

    for (auto &[ignore, field] : stru) {
        temp = encoder.encode(*field, temp, len - offset);
        offset += field->capacity();
    }

    return ptr + offset;
}

State StrcutDecoder::decode(Struct &stru, const void *ptr, size_t len)
{
    FieldEncoder decoder;

    auto data = reinterpret_cast<const tlv::byte *>(ptr);
    auto state = tlv::State::NotReady;

    size_t offset = 0;
    while (offset < len) {
        Field field;
        state = decoder.decode(field, data + offset, len - offset);
        if (state != tlv::State::Success) {
            return state;
        }

        offset += field.capacity();

        auto tag = field.tag();
        stru[tag] = std::make_shared<Field>(std::move(field));
    }

    return state;
}
