#pragma once

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string>

using namespace std;

class cryptinfo {
	uint8_t key[32];
	uint8_t iv[AES_BLOCK_SIZE];
	EVP_CIPHER_CTX *ectx;
	EVP_CIPHER_CTX *dctx;

	void fillectx()
	{
		debug("getting new encryption params");
		debug("making new ctx, pass me a unfilled one");
		if (!(ectx = EVP_CIPHER_CTX_new())) handleErrors();
		debug("initializing the encryption operation");
		if (1 != EVP_EncryptInit_ex(ectx, EVP_aes_256_cbc(), NULL, NULL, NULL)) handleErrors();
		debug("initialize key and IV", "\nbefore: ", key, " ", iv);
		if (1 != EVP_EncryptInit_ex(ectx, NULL, NULL, key, iv)) handleErrors();
		debug("read key and IV", "\nafter: ", key, " ", iv);
	}

	void filldctx()
	{
		debug("getting new decryption params");
		debug("making new ctx, pass me a unfilled one");
		if (!(dctx = EVP_CIPHER_CTX_new())) handleErrors();
		debug("initializing the decryption operation");
		if (!EVP_DecryptInit_ex(dctx, EVP_aes_256_cbc(), NULL, NULL, NULL)) handleErrors();
		debug("setting dctx key and iv \nbefore ", key, " ", iv);
		if (!EVP_DecryptInit_ex(dctx, NULL, NULL, key, iv)) handleErrors();
	}

	void filldectx()
	{
		fillectx();
		filldctx();
	}

	void handleErrors(void)
	{
		unsigned long errCode;
		debug("An error has occured in encryption/decryption");
		while (errCode = ERR_get_error())
			debug(ERR_error_string(errCode, NULL));
		abort();
	}
	
	friend char* operator<<(char* os, cryptinfo  &e);
	friend char* operator>>(char* os, cryptinfo  &e);
	friend std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e);
	friend std::istream& operator>>(std::istream &os, cryptinfo  &e);


public:
	void fillkeyiv()
	{
		debug("creating new key and iv for a cryptinfo");
		RAND_bytes(key, sizeof(key));
		RAND_bytes(iv, sizeof(iv));
	}

	int encrypt(std::string& plainstr, std::string& ciphertext, unsigned char* aad = 0, int aad_len = 0)
	{
		fillectx();
		debug("encrypting string ", plainstr);
		int len;
		int ciphertextlen;
		unsigned char ciphertostr[48];
		debug("writing encrypted bytes to ciphertext");
		if (1 != EVP_EncryptUpdate(ectx, ciphertostr, &len, (const unsigned char*)plainstr.c_str(), plainstr.size())) handleErrors();
		ciphertextlen = len;
		debug("writing final bytes to ciphertext");
		if (1 != EVP_EncryptFinal_ex(ectx, ciphertostr + ciphertextlen, &len)) handleErrors();
		ciphertextlen += len;
		ciphertext[ciphertextlen] = '\0';
		debug("encrypted string ", ciphertext);
		debug("free ctx obj");
		EVP_CIPHER_CTX_free(ectx);
		debug("return ciphertext_len");
		ciphertext = (const char*)ciphertostr;
		return ciphertextlen;
	}

	int decrypt(std::string& ciphertext, std::string& plainstr, unsigned char *aad = 0, int aad_len = 0)
	{
		filldctx();
		debug("decrypting string ", ciphertext);
		int len;
		int plaintextlen;
		unsigned char buf[48];
		int ret;
		debug("writing unencrypted bytes to plaintext");
		if (!EVP_DecryptUpdate(dctx, buf, &len, (const unsigned char*)ciphertext.c_str(), ciphertext.size()))
			handleErrors();
		plaintextlen = len;
		debug("writing final bytes to plaintext");
		ret = EVP_DecryptFinal_ex(dctx, buf + plaintextlen, &len);
		plaintextlen += len;
		*(buf + plaintextlen) = '\0';
		debug("freeing ctx obj");
		EVP_CIPHER_CTX_free(dctx);
		if (ret == 1)
		{
			debug("decrypt success\ndecrypted string: ", buf);
			debug("returning plain text len");
			plainstr = (const char*)buf;
			return plaintextlen;
		}
		else
		{
			debug("there may have been an error check != -1");
			plainstr = (const char*)buf;
			return -1;
		}
	}
};

std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e)
{
	for (int i = 0; i < 32; i++)
	{
		os << e.key[i];
	}
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
	{
		os << e.iv[i];
	}
	return os;
}

std::istream& operator>>(std::istream &os, cryptinfo  &e)
{
	for (int i = 0; i < 32; i++)
	{
		os >> e.key[i];
	}
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
	{
		os >> e.iv[i];
	}
	return os;
}

char* operator>>(char* os, cryptinfo  &e)
{
	for (int i = 0; i < 32; i++)
	{
		e.key[i] = os[i];
	}
	for (int i = 32; i < 32 + AES_BLOCK_SIZE; i++)
	{
		e.iv[i] = os[i];
	}
	return os;
}

char* operator<<(char* os, cryptinfo  &e)
{
	for (int i = 0; i < 32; i++)
	{
		os[i] = e.key[i];
	}
	for (int i = 32; i < 32 + AES_BLOCK_SIZE; i++)
	{
		os[i] = e.iv[i];
	}
	return os;
}