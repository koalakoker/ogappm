#if !defined(GCRYPT_)
#define GCRYPT_

void GCrypt_Initialize(const char* keyValue,unsigned int length);
void GCrypt(const char* command,unsigned char* testo, unsigned int n);
void GHashPass(const unsigned char* pass,unsigned int n,unsigned int* pH, unsigned int* pL);

#endif // !defined(GCRYPT_)
