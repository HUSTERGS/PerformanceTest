#include <iostream>
#include "viper/viper.hpp"
#include <gtest/gtest.h>
#include <mutex>
#include <filesystem>

std::mutex m;
TEST(a, b) {
    std::filesystem::remove_all("/pmem-fs/viper");
  const size_t initial_size = 1 << 30;  // 1 GiB
  auto viper_db = viper::Viper<std::string, std::string>::create("/pmem-fs/viper", initial_size);
  
  // To modify records in Viper, you need to use a Viper Client.
  auto v_client = viper_db->get_client();
  
  // for (uint64_t key = 0; key < 10; ++key) {
  //   const uint64_t value = key + 10;
  //   v_client.put(std::to_string(key), std::to_string(value));
  // }
  
  // for (uint64_t key = 0; key < 11; ++key) {
  //   std::string value;
  //   const bool found = v_client.get(std::to_string(key), &value);
  //   if (found) {
  //     std::cout << "key: " << key << "value: " << value << std::endl;
  //   } else {
  //     std::cout << "No record found for key: " << key << std::endl;
  //   }
  // }
  std::string a = "1234";
  v_client.put(a, std::string("value"));
  a.clear();
  a.append("4567");
  std::string b = "1234";
  std::string value;
  v_client.get(b, &value);
  EXPECT_EQ(value , "value");
  std::filesystem::remove_all("/pmem-fs/viper");
}

void t(int i) {
    std::lock_guard<std::mutex> guard(m);
    // usleep(1);
    std::this_thread::sleep_for(std::chrono::seconds(i));
}

int main(int argc, char **argv)
{
	// auto start = std::chrono::high_resolution_clock::now();
  // std::thread t1(t, 1);
  // std::thread t2(t, 2);
  // std::thread t3(t, 3);
  // std::thread t4(t, 4);
  // t1.join();
  // t2.join();
  // t3.join();
  // t4.join();
  // auto end = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double> diff = end-start;
  // std::cout <<  "time: " << diff.count() << std::endl;
  // return 0;
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
