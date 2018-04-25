#include <iostream>
#include <dlfcn.h>

using AES_128_encrypt_t = uint64_t(*)(unsigned char* plaintext, unsigned char* ciphertext);

int main(void) {
  void* h = dlopen("./libnative-lib.so", RTLD_NOW);

  if (h == nullptr) {
    std::cerr << "Unable to load the library" << std::endl;
    return 1;
  }

  void* sh = dlsym(h, "_Z48TfcqPqf1lNhu0DC2qGsAAeML0SEmOBYX4jpYUnyT8qYWIlEqPhS_");
  if (sh == nullptr) {
    std::cerr << "Unable to get function" << std::endl;
    return 1;
  }

  AES_128_encrypt_t AES_128_encrypt = reinterpret_cast<AES_128_encrypt_t>(sh);
  unsigned char plaintext[16];
  unsigned char ciphertext[16];
  if (fread(plaintext, 1, 16, stdin) != 16)
    return 1;
  AES_128_encrypt(plaintext, ciphertext);
  fwrite(ciphertext, 1, 16, stdout);
  return 0;
}
