#include <iostream>

#include "tlv_field.h"
#include "tlv_struct.h"
#include "tlv_decoder.h"

int main(int argc, char *argv[])
{
    int i = 123456;
    const char *str1 = "12321";
    std::string str2{"67890"};

    int arr[3]{7, 2, 3};

    tlv::Struct stru1{
        tlv::make_field(0x01, arr),
        tlv::make_field(0x02, i),
        tlv::make_field(0x03, 7890),
        tlv::make_field(0x04, str1),
        tlv::make_field(0x05, str2),
        tlv::make_field(0x06, "0987654")
    };

    tlv::StrcutDecoder decoder;
    auto ptr = tlv::allocate<tlv::byte>(stru1.capacity());
    decoder.encode(stru1, ptr.get(), stru1.capacity());

    tlv::Struct stru2;
    decoder.decode(stru2, ptr.get(), stru1.capacity());

    for (auto &[ignore, field] : stru2) {
         std::cout << int(field->tag()) << " " << field->size() << std::endl;
    }

    std::cout << "-------------" << std::endl;

    auto arr1 = stru2[0x1]->value<const int *>();
    std::cout << arr1[0] << arr1[1] << arr1[2] << std::endl;

    auto i1 = stru2[0x2]->value<int>();
    std::cout << i1 << std::endl;

    auto i2 = stru2[0x3]->value<int>();
    std::cout << i2 << std::endl;

    auto str3 = stru2[0x04]->value<const char *>();
    std::cout << str3 << std::endl;

    auto str4 = stru2[0x05]->value<const char *>();
    std::cout << str4 << std::endl;

    auto str5 = stru2[0x06]->value<const char *>();
    std::cout << str5 << std::endl;

    std::cout << "-------------" << std::endl;

    tlv::Struct stru3;
    stru3.emplace(0x01, arr);
    stru3.emplace(0x02, i);
    stru3.emplace(0x03, 7890),
    stru3.emplace(0x04, str1);
    stru3.emplace(0x05, str2);
    stru3.emplace(0x06, "0987654");

    for (auto &[ignore, field] : stru3) {
         std::cout << int(field->tag()) << " " << field->size() << std::endl;
    }

    std::cout << "-------------" << std::endl;

    auto arr2 =  stru3[0x1]->value<const int *>();
    std::cout << arr2[0] << arr2[1] << arr2[2] << std::endl;

    auto i3 = stru3[0x2]->value<int>();
    std::cout << i3 << std::endl;

    auto i4 = stru3[0x3]->value<int>();
    std::cout << i4 << std::endl;

    auto str6 = stru3[0x04]->value<const char *>();
    std::cout << str6 << std::endl;

    auto str7 = stru3[0x05]->value<const char *>();
    std::cout << str7 << std::endl;

    auto str8 = stru3[0x06]->value<const char *>();
    std::cout << str8 << std::endl;

    return 0;
}
