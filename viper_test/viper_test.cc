#include <iostream>
#include "viper/viper.hpp"
#include <gtest/gtest.h>


TEST(a, b) {
const size_t initial_size = 1 << 29;  // 1 GiB
  auto viper_db = viper::Viper<std::string, std::string>::create("/pmem-fs/viper", initial_size);
  
  // To modify records in Viper, you need to use a Viper Client.
  auto v_client = viper_db->get_client();
  
  for (uint64_t key = 0; key < 10; ++key) {
    const uint64_t value = key + 10;
    v_client.put(std::to_string(key), std::to_string(value));
  }
  
  for (uint64_t key = 0; key < 11; ++key) {
    std::string value;
    const bool found = v_client.get(std::to_string(key), &value);
    if (found) {
      std::cout << "key: " << key << "value: " << value << std::endl;
    } else {
      std::cout << "No record found for key: " << key << std::endl;
    }
  }
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
