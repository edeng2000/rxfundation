#include "encrypt/rxdesopenssl.h"
#include "rxbase64coding.hpp"
#include "openssl/des.h"
#include <string.h>
#include <vector>
typedef  unsigned char  BYTE;

const string m_desKey = "QAZwsx!@";
#define  MAX_DES_DATA_LEN        64*1024

//加密 cbc pkcs5padding 自己实现  //pkcs7padding 跟 pkcs5padding是一样的
std::string des_cbc_pkcs5_encrypt(const std::string& clearText, const std::string& key)
{
	static unsigned char cbc_iv[8] = { '1', '2', '3', '4', '5', '6', '7', '8' };
	//初始化IV向量 
	std::string strCipherText;
	DES_cblock keyEncrypt, ivec;
	memset(keyEncrypt, 0, 8);

	if (key.length() <= 8)
		memcpy(keyEncrypt, key.c_str(), key.length());
	else
		memcpy(keyEncrypt, key.c_str(), 8);

	DES_key_schedule keySchedule;  //密钥表
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);   //设置密钥，且不检测密钥奇偶性  

	memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	// 循环加密，每8字节一次    
	const_DES_cblock inputText;
	DES_cblock outputText;
	std::vector<unsigned char> vecCiphertext;
	unsigned char tmp[8];

	for (int i = 0; i < (int)clearText.length() / 8; i++)
	{
		memcpy(inputText, clearText.c_str() + i * 8, 8);
		DES_ncbc_encrypt(inputText, outputText, 8, &keySchedule, &ivec, DES_ENCRYPT);  //加密
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
			vecCiphertext.push_back(tmp[j]);

		//重置ivec
		memcpy(ivec, outputText, 8);
	}

	if (clearText.length() % 8 != 0)
	{
		int tmp1 = (int)(clearText.length() / 8 * 8);
		int tmp2 = (int)(clearText.length() - tmp1);
		memset(inputText, 0, 8);
		memcpy(inputText, clearText.c_str() + tmp1, tmp2);
	}
	else
	{
		memset(inputText, 8, 8);
	}
	// 加密函数    
	DES_ncbc_encrypt(inputText, outputText, 8, &keySchedule, &ivec, DES_ENCRYPT);  //加密 
	memcpy(tmp, outputText, 8);

	for (int j = 0; j < 8; j++)
		vecCiphertext.push_back(tmp[j]);

	strCipherText.clear();
	strCipherText.assign(vecCiphertext.begin(), vecCiphertext.end());
	return strCipherText;
}


//解密 cbc pkcs5padding 自己实现  //zeropadding / pkcs7padding 跟 pkcs5padding是一样的 
std::string des_cbc_pkcs5_decrypt(const std::string& cipherText, const std::string& key)
{
	static unsigned char cbc_iv[8] = { '1', '2', '3', '4', '5', '6', '7', '8' };
	//初始化IV向量 
	std::string clearText;
	DES_cblock keyEncrypt, ivec;
	memset(keyEncrypt, 0, 8);

	if (key.length() <= 8)
		memcpy(keyEncrypt, key.c_str(), key.length());
	else
		memcpy(keyEncrypt, key.c_str(), 8);

	DES_key_schedule keySchedule;  //密钥表
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);   //设置密钥，且不检测密钥奇偶性  

	memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	// 循环解密，每8字节一次    
	const_DES_cblock inputText;
	DES_cblock outputText;
	std::vector<unsigned char> vecCleartext;
	unsigned char tmp[8];
	
	for (int i = 0; i < (int)cipherText.length() / 8; i++)
	{
		memcpy(inputText, cipherText.c_str() + i * 8, 8);
		memset(&outputText, 0,sizeof(DES_cblock));
		DES_ncbc_encrypt(inputText, outputText, 8, &keySchedule, &ivec, DES_DECRYPT);  //解密
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
			vecCleartext.push_back(tmp[j]);

		//重置ivec
		//memcpy(ivec, outputText, 8);  //解密过程不需要用前一块的结果作为下一块的IV
	}

	if (clearText.length() % 8 != 0)
	{
		int tmp1 = (int)(clearText.length() / 8 * 8);
		int tmp2 = (int)(clearText.length() - tmp1);
		memset(inputText, 0, tmp2);
		memcpy(inputText, cipherText.c_str() + tmp1, tmp2);
		DES_ncbc_encrypt(inputText, outputText, tmp2, &keySchedule, &ivec, DES_DECRYPT);  //解密
		memcpy(tmp, outputText, tmp2);
		for (int j = 0; j < 8; j++)
			vecCleartext.push_back(tmp[j]);
	}
	clearText.clear();
	clearText.assign(vecCleartext.begin(), vecCleartext.end());
	return clearText;
}

CRXDesOpenssl::CRXDesOpenssl(void)
{
}

CRXDesOpenssl::~CRXDesOpenssl(void)
{

}

// ---- des对称加解密 ---- //    
// 加密 ecb模式    
std::string CRXDesOpenssl::des_encrypt(const std::string& clearText)
{

 	string strOut = des_cbc_pkcs5_encrypt(clearText, m_desKey);
 	return base64_encode((BYTE*)strOut.c_str(), (uint32)strOut.size());
}

// 解密 ecb模式    
std::string CRXDesOpenssl::des_decrypt(const std::string & cipherText)
{
	if (cipherText.size()==0)
	{
		return cipherText;
	}
 	std::string clearText = base64_decode(cipherText); // 明文    
	string strOut = des_cbc_pkcs5_decrypt(clearText, m_desKey);
 	return strOut;
}


std::string CRXDesOpenssl::des_encrypt_data(const std::string& clearText)
{
	string strOut = des_cbc_pkcs5_encrypt(clearText, m_desKey);
	return base64_encode((BYTE*)strOut.c_str(), (uint32)strOut.size());	
}

std::string CRXDesOpenssl::des_decrypt_data(const std::string& cipherText)
{
 	std::string clearText = base64_decode(cipherText); // 明文    
	string strOut = des_cbc_pkcs5_decrypt(cipherText, m_desKey);
	return strOut;
}