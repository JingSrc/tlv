#ifndef TLV_STRUCT_H
#define TLV_STRUCT_H

#include <unordered_map>
#include <optional>

#include "tlv_field.h"

namespace tlv {

class Struct {
public:
    using container_type  = std::unordered_map<Field::tag_type, std::shared_ptr<Field>>;
    using key_type        = container_type::key_type;
    using mapped_type     = container_type::mapped_type;
    using value_type      = container_type::value_type;
    using reference       = container_type::reference;
    using const_reference = container_type::const_reference;
    using iterator        = container_type::iterator;
    using const_iterator  = container_type::const_iterator;
    using size_type       = container_type::size_type;
    using difference_type = container_type::difference_type;

    Struct() = default;
    ~Struct() = default;

    explicit Struct(std::initializer_list<mapped_type::element_type> fields);
    explicit Struct(std::initializer_list<mapped_type> fields);

    Struct(const Struct &other);
    Struct(Struct &&other);

    Struct &operator=(const Struct &other);
    Struct &operator=(Struct &&other);

    inline iterator begin() noexcept {
        return mFields.begin();
    }
    inline iterator end() noexcept {
        return mFields.end();
    }
    inline const_iterator begin() const noexcept {
        return mFields.begin();
    }
    inline const_iterator end() const noexcept {
        return mFields.end();
    }

    inline bool empty() const {
        return mFields.empty();
    }

    size_type size() const {
        return mFields.size();
    }

    int capacity() const;

    inline void insert(const mapped_type::element_type &field) {
        mFields.insert({field.tag(), std::make_shared<Field>(field)});
    }
    inline void insert(mapped_type::element_type &&field) {
        mFields.insert({field.tag(), std::make_shared<Field>(std::move(field))});
    }
    inline void insert(mapped_type field) {
        mFields.insert({field->tag(), field});
    }

    template <typename T> inline void emplace(Field::tag_type tag, const T &data) {
        if constexpr(std::is_same_v<const char *, T>) {
            insert(std::make_shared<Field>(tag, Field::size_type(std::strlen(data)), data));
        } else {
            static_assert(!std::is_pointer_v<T>, "can not be a pointer");
            insert(std::make_shared<Field>(tag, sizeof(data), &data));
        }
    }

    template <> inline void emplace(Field::tag_type tag, const std::string &data) {
        insert(std::make_shared<Field>(tag, Field::size_type(data.size()), data.data()));
    }

    template <typename T, Field::size_type N> inline void emplace(Field::tag_type tag, const T (&arr)[N]) {
        if constexpr(std::is_same_v<T, char>) {
            emplace(tag, reinterpret_cast<const char *>(arr));
        } else {
            insert(std::make_shared<Field>(tag, Field::size_type(N * sizeof(T)), &arr[0]));
        }
    }

    inline iterator find(key_type tag) {
        return mFields.find(tag);
    }

    inline iterator erase(iterator it) {
        return mFields.erase(it);
    }
    void remove(Field::tag_type tag);

    mapped_type &operator[](Field::tag_type tag);

    void swap(Struct &other);

private:
    container_type mFields;
};

}

#endif // TLV_STRUCT_H
