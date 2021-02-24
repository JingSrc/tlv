#include "tlv_private.h"
#include "tlv_field.h"

using namespace tlv;

Field::Field(tag_type tag, int size)
{
    reset(tag, size, nullptr);
}

Field::Field(tag_type tag, int size, const void *data)
{
    reset(tag, size, data);
}

Field::Field(const Field &other)
{
    *this = other;
}

Field::Field(Field &&other)
{
    if (this != &other) {
        swap(other);
        other.mData.reset();
    }
}

Field &Field::operator=(const Field &other)
{
    if (this != &other) {
        reset(other.tag(), other.size(), other.value());
    }

    return *this;
}

Field &Field::operator=(Field &&other)
{
    if (this != &other) {
        swap(other);
        other.mData.reset();
    }

    return *this;
}

Field::tag_type Field::tag() const
{
    return mData ? mData->tag : 0;
}

Field::size_type Field::size() const
{
    return mData ? mData->size : 0;
}

const Field::value_type *Field::value() const
{
    return mData ? mData->value : nullptr;
}

Field::size_type Field::capacity() const
{
    return sizeof(internal::FieldPrivate) + size();
}

void Field::swap(Field &other)
{
    if (this != &other) {
        mData.swap(other.mData);
    }
}

void Field::reset(tag_type tag, size_type size, const void *data)
{
    if (size != this->size()) {
        auto len = sizeof(internal::FieldPrivate) + size + 1;
        mData = allocate<internal::FieldPrivate>(len);
        reinterpret_cast<char *>(mData.get())[len - 1] = '\0';
    }

    mData->tag = tag;
    mData->size = size;
    if (data != nullptr) {
        std::memcpy(mData->value, data, size);
    }
}
