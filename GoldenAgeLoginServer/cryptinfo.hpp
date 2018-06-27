#pragma once

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <string>

struct cryptinfo {
	uint8_t key[32];
	uint8_t iv[AES_BLOCK_SIZE];
	EVP_CIPHER_CTX *ectx;
	EVP_CIPHER_CTX *dctx;

private:
	friend std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e);
	friend std::istream& operator>>(std::istream &os, cryptinfo  &e);
};

std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e)
{
	for (int i = 0; i < 32; i++)
	{
		os << e.key[i];
	}
	os << "\n";
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
	{
		os << e.iv[i];
	}
	os << "\n";

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