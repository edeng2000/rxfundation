#ifndef ___RX_FUNDATION_ENCRYPT_DES_INCLUDE_
#define ___RX_FUNDATION_ENCRYPT_DES_INCLUDE_
#include <string>
using namespace std;

class CRXDesOpenssl
{
public:
	CRXDesOpenssl(void);
	~CRXDesOpenssl(void);
public:
	static std::string des_encrypt(const std::string& clearText);
	static std::string des_decrypt(const std::string& cipherText);
	static std::string  des_encrypt_data(const std::string& clearText);
	static std::string  des_decrypt_data(const std::string& clearText);
};
#endif // !_ENCRYPT_DES_INCLUDE_

