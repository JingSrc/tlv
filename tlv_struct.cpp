#include "tlv_struct.h"

using namespace tlv;

Struct::Struct(std::initializer_list<mapped_type::element_type> fields)
{
    for (auto &field : fields) {
        insert(field);
    }
}

Struct::Struct(std::initializer_list<Struct::mapped_type> fields)
{
    for (auto &field : fields) {
        insert(field);
    }
}

Struct::Struct(const Struct &other)
{
    *this = other;
}

Struct::Struct(Struct &&other)
{
    if (this != &other) {
        swap(other);
        container_type().swap(other.mFields);
    }
}

Struct &Struct::operator=(const Struct &other)
{
    if (this != &other) {
        mFields.clear();
        for (auto &[ignore, field] : other) {
            insert(field);
        }
    }

    return *this;
}

Struct &Struct::operator=(Struct &&other)
{
    if (this != &other) {
        swap(other);
        container_type().swap(other.mFields);
    }

    return *this;
}

int Struct::capacity() const
{
    int total = 0;

    for (auto &[tag, field] : mFields) {
        total += field->capacity();
    }

    return total;
}

void Struct::remove(Field::tag_type tag)
{
    auto it = mFields.find(tag);
    if (it != mFields.end()) {
        mFields.erase(it);
    }
}

Struct::mapped_type &Struct::operator [](Field::tag_type tag)
{
    return mFields[tag];
}

void Struct::swap(Struct &other)
{
    if (this != &other) {
        mFields.swap(other.mFields);
    }
}
