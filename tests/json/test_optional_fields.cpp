#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_optional_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  rfl::Rename<"children", std::optional<std::vector<Person>>> children;
};


struct optional_test_t {
    int i;
    std::optional<int> opt_i;
    int j;
    std::optional<int> opt_j;
    double d;
    std::optional<double> opt_d;
};


struct optional_test_strings_t {
    std::string str;
    std::optional<std::string> opt_str;
};

TEST(json, test_simple_optional_fields) {

  const optional_test_t test =
    optional_test_t{
        .i = 42,
        .opt_i = 12345,
        .j = 69,
        .d = 42.69,
    };
  // const optional_test_strings_t test_strings =
  //   optional_test_strings_t {
  //       .str = "non-optional string",
  //       .opt_str = "optional string",
  //   };

  // std::cout << std::hex << std::setw(2) << std::setfill('0') << std::hex;

  // rfl::cbor::write(test, std::cout << std::hex << std::setw(2) << std::setfill('0') << std::hex) << std::endl;
  // std::cout<<std::endl;
  // std::cout<<std::dec;

  std::string json = rfl::json::write(test);

  std::cout<<json<<std::endl;

  // write_and_read(test);
}

TEST(json, test_optional_fields) {
  const auto bart = Person{.first_name = "Bart"};

  const auto lisa = Person{.first_name = "Lisa"};

  const auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson"},{"firstName":"Lisa","lastName":"Simpson"},{"firstName":"Maggie","lastName":"Simpson"}]})");
}
}  // namespace test_optional_fields
