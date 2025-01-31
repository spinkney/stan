#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "glaze/glaze.hpp"

struct TestData {
    int id;
    std::string name;
};

template <>
struct glz::meta<TestData> {
    static constexpr auto value = glz::object(
        "id", &TestData::id,
        "name", &TestData::name
    );
};

int main() {
    TestData data = {42, "Stan Model"};

    std::vector<uint8_t> buffer;

    auto ec = glz::write_beve(data, buffer);

    if (bool(ec)) {
        std::cerr << "Serialization failed: " << glz::format_error(ec, buffer) << std::endl;
        return 1;
    }

    std::ofstream file("test_output.beve", std::ios::binary);
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());

    std::cout << "BEVE serialization successful! Output written to test_output.beve\n";
    return 0;
}

