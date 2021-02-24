#include <iostream>

#include "tlv_field.h"
#include "tlv_struct.h"
#include "tlv_decoder.h"

int main(int argc, char *argv[])
{
    int i = 1534672346;
    const char *str1 = "12321";
    std::string str2{"67890"};

    int arr[3]{7, 2, 3};

    tlv::Struct stru{
        tlv::make_field(0x01, arr),
        tlv::make_field(0x03, i),
        tlv::make_field(0x04, str1),
        tlv::make_field(0x05, str2),
        tlv::make_field(0x06, "0987654")
    };

    tlv::StrcutDecoder decoder;
    auto ptr = tlv::allocate<tlv::byte>(stru.capacity());
    decoder.encode(stru, ptr.get(), stru.capacity());

    tlv::Struct stru1;
    decoder.decode(stru1, ptr.get(), stru.capacity());

    for (auto &[ignore, field] : stru1) {
         std::cout << field->tag() << field->size() << std::endl;
    }

    auto arr1 = tlv::get_field<const int *>(stru1[0x1]);
    std::cout << arr1[0] << arr1[1] << arr1[2] << std::endl;

    auto i1 = tlv::get_field<int>(stru1[0x3]);
    std::cout << i1 << std::endl;

    auto str3 = tlv::get_field<const char *>(stru1[0x04]);
    std::cout << str3 << std::endl;

    auto str4 = tlv::get_field<const char *>(stru1[0x05]);
    std::cout << str4 << std::endl;

    auto str5 = tlv::get_field<const char *>(stru1[0x06]);
    std::cout << str5 << std::endl;

    return 0;
}
