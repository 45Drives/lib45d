#include <45d/Quota.hpp>
#include <string>
#include <cassert>

void test_construction(void) {
    ffd::Bytes a(100);
    
    std::cout << "max = " << a << std::endl;
    std::cout << "Testing construction from \"50%\" == from 0.5 == from max/2 == from bytes string == " << a.get() / 2 << std::endl;
    ffd::Quota str_quota(a, "50%");
    ffd::Quota double_quota(a, 0.5);
    ffd::Quota bytes_quota(a, a/2);
    ffd::Quota bytes_str_quota(a, (a/2).get_str());
    assert(str_quota == double_quota && double_quota == bytes_quota && bytes_quota == bytes_str_quota && bytes_str_quota == a.get() / 2);
    std::cout << "OK" << std::endl;
}

void test_math(void) {
    ffd::Bytes bytes(1000); // 1 KB
    ffd::Quota quota(bytes); // 100% initially

    std::cout << "asserting 100% of " << bytes << " is " << bytes << std::endl;
    assert(bytes == quota);
    std::cout << "OK" << std::endl;

    std::cout << "asserting 50% of " << bytes << " is " << bytes << " / 2" << std::endl;
    quota.set_fraction(0.5);
    assert(quota == bytes / 2);
    std::cout << "OK" << std::endl;

    std::cout << "asserting (quota)" << quota << " / (bytes)" << bytes << " is 0.5" << std::endl;
    assert(quota / bytes == 0.5);
    std::cout << "OK" << std::endl;

    std::cout << "asserting (bytes)" << bytes << " / (quota)" << quota << " is 2.0" << std::endl;
    assert(bytes / quota == 2.0);
    std::cout << "OK" << std::endl;
}

void test_string(void) {
    ffd::Bytes bytes(1000); // 1 KB
    ffd::Quota quota(bytes, 0.5);

    std::cout << "asserting 50% of " << bytes.get_str(ffd::Bytes::PrefixType::SI, 0) << " is " << (bytes / 2).get_str(ffd::Bytes::PrefixType::SI, 0) << std::endl;
    assert(quota.get_str(ffd::Bytes::PrefixType::SI, 0) == (bytes / 2).get_str(ffd::Bytes::PrefixType::SI, 0));
    std::cout << "OK" << std::endl;
}

void test_rounding(void) {
    ffd::Quota quota({100}, 1.0/3.0); // 1/3 of 100 B

    std::cout << "asserting 100 / 3 rounding nearest is 33" << std::endl;
    quota.set_rounding_method(ffd::Quota::RoundingMethod::NEAREST);
    assert(quota.get() == ffd::Bytes::bytes_type(33));
    std::cout << "OK" << std::endl;

    std::cout << "asserting 100 / 3 rounding down is 33" << std::endl;
    quota.set_rounding_method(ffd::Quota::RoundingMethod::DOWN);
    assert(quota.get() == ffd::Bytes::bytes_type(33));
    std::cout << "OK" << std::endl;

    std::cout << "asserting 100 / 3 rounding up is 34" << std::endl;
    quota.set_rounding_method(ffd::Quota::RoundingMethod::UP);
    assert(quota.get() == ffd::Bytes::bytes_type(34));
    std::cout << "OK" << std::endl;

    quota = ffd::Quota({100}, 2.0/3.0); // 2/3 of 100 B

    std::cout << "asserting 100 * 2 / 3 rounding nearest is 67" << std::endl;
    quota.set_rounding_method(ffd::Quota::RoundingMethod::NEAREST);
    assert(quota.get() == ffd::Bytes::bytes_type(67));
    std::cout << "OK" << std::endl;

    std::cout << "asserting 100 * 2 / 3 rounding down is 66" << std::endl;
    quota.set_rounding_method(ffd::Quota::RoundingMethod::DOWN);
    assert(quota.get() == ffd::Bytes::bytes_type(66));
    std::cout << "OK" << std::endl;

    std::cout << "asserting 100 * 2 / 3 rounding up is 67" << std::endl;
    quota.set_rounding_method(ffd::Quota::RoundingMethod::UP);
    assert(quota.get() == ffd::Bytes::bytes_type(67));
    std::cout << "OK" << std::endl;
}

int main() {
    test_construction();
    test_math();
    test_string();
    test_rounding();
    return 0;
}