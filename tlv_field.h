#ifndef TLV_FIELD_H
#define TLV_FIELD_H

#include <string>
#include <string_view>

#include "tlv_global.h"

namespace tlv {

namespace internal {
struct FieldPrivate;
}

class Field {
public:
    using tag_type = tlv::byte;
    using value_type = tlv::byte;
    using size_type = int;

    Field() = default;
    ~Field() = default;

    Field(tag_type tag, int size);
    Field(tag_type tag, int size, const void *data);

    Field(const Field &other);
    Field(Field &&other);

    Field &operator=(const Field &other);
    Field &operator=(Field &&other);

    tag_type tag() const;
    size_type size() const;
    const value_type *value() const;

    size_type capacity() const;

    void swap(Field &other);
    void reset(tag_type tag, size_type size, const void *data);

private:
    std::shared_ptr<internal::FieldPrivate> mData;
};

template <typename T> inline std::shared_ptr<Field> make_field(Field::tag_type tag, const T &data) {
    if constexpr(std::is_same_v<const char *, T>) {
        return std::make_shared<Field>(tag, Field::size_type(std::strlen(data)), data);
    } else {
        static_assert(!std::is_pointer_v<T>, "can not be a pointer");
        return std::make_shared<Field>(tag, sizeof(data), &data);
    }
}

//template <typename T> inline std::shared_ptr<Field> make_field(Field::tag_type tag, const T *data);

//template <> inline std::shared_ptr<Field> make_field(Field::tag_type tag, const char *data) {
//    return std::make_shared<Field>(tag, Field::size_type(std::strlen(data)), data);
//}

template <> inline std::shared_ptr<Field> make_field(Field::tag_type tag, const std::string &data) {
    return std::make_shared<Field>(tag, Field::size_type(data.size()), data.data());
}

template <typename T, Field::size_type N> inline std::shared_ptr<Field> make_field(Field::tag_type tag, const T (&arr)[N]) {
    if constexpr(std::is_same_v<T, char>) {
        return make_field(tag, reinterpret_cast<const char *>(arr));
    } else {
        return std::make_shared<Field>(tag, Field::size_type(N * sizeof(T)), &arr[0]);
    }
}

template <typename T> inline T get_field(const Field &field) {
    if constexpr(std::is_pointer_v<T>) {
        return reinterpret_cast<T>(field.value());
    } else {
        return *reinterpret_cast<const T *>(field.value());
    }
}

template <typename T> inline T get_field(std::shared_ptr<Field> field) {
    return get_field<T>(*field);
}

}

#endif // TLV_FIELD_H
