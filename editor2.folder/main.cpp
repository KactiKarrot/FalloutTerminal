#include <string>
#include <iostream>
#include <dlfcn.h>

void run_so(std::string fileName) {
  void* handle = dlopen(fileName.c_str(), RTLD_LAZY);
  typedef void (*test_t)();
  test_t start = (test_t) dlsym(handle, "start");
  start();
}

int main () {
  //std::string test = "testText";
  //std::cout << test << std::endl;
  run_so("/usr/lib/myLib.so");
  return 0;
}