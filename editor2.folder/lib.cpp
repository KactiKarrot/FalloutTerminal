#include <string>
#include <iostream>
#include <vector>

void testFunc(std::string &str) {
  str = "aaaa";
  std::cout << str << std::endl;
}

extern "C" int start() {
  std::vector<std::string> test;
  test.push_back("testText");
  std::cout << test[0] << std::endl;
  testFunc(test[0]);
  std::cout << test[0] << std::endl;
  return 0;
}