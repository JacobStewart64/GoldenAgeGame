#pragma once
#include <GoldenAge/debug.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string>

namespace ga {
	class cryptinfo {
		EVP_CIPHER_CTX* ectx;
		EVP_CIPHER_CTX* dctx;

		void fillectx()
		{
			debug("making new encryption ctx");
			if (!(ectx = EVP_CIPHER_CTX_new())) handleErrors();
			debug("setting ctx encryption engine to EVP_aes_256_cbc()");
			if (1 != EVP_EncryptInit_ex(ectx, EVP_aes_256_cbc(), NULL, NULL, NULL)) handleErrors();
			debug("initializing ctx with key and iv", key, " ", iv);
			if (1 != EVP_EncryptInit_ex(ectx, NULL, NULL, key, iv)) handleErrors();
			debug("making new encryption ctx good");
		}

		void filldctx()
		{
			debug("making new decryption ctx");
			if (!(dctx = EVP_CIPHER_CTX_new())) handleErrors();
			debug("setting ctx decryption engine to EVP_aes_256_cbc()");
			if (!EVP_DecryptInit_ex(dctx, EVP_aes_256_cbc(), NULL, NULL, NULL)) handleErrors();
			debug("initializing ctx with key and iv");
			if (!EVP_DecryptInit_ex(dctx, NULL, NULL, key, iv)) handleErrors();
			debug("making new decryption ctx good");
		}

		void filldectx()
		{
			debug("make both encryption and decryption ctx at the same time");
			fillectx();
			filldctx();
			debug("make both good");
		}

		void handleErrors(void)
		{
			unsigned long errCode;
			debug("An error has occured in encryption/decryption");
			while (errCode = ERR_get_error())
				debug(ERR_error_string(errCode, NULL));
			debug("should we really exit here?");
			system("pause");
			abort();
		}
	
		friend char* operator<<(char* os, cryptinfo  &e);
		friend char* operator>>(char* os, cryptinfo  &e);
		friend std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e);
		friend std::istream& operator>>(std::istream &os, cryptinfo  &e);

	public:
		uint8_t key[32];
		uint8_t iv[AES_BLOCK_SIZE];

		void fillkeyiv()
		{
			debug("creating new key and iv for a cryptinfo");
			debug("creating key");
			RAND_bytes(key, sizeof(key));
			debug("creating iv");
			RAND_bytes(iv, sizeof(iv));
			debug("creating new key and iv good");
		}

		int encrypt(std::string& encrypt_in, std::string& encrypt_out, unsigned char* aad = 0, int aad_len = 0)
		{
			debug("encrypt called");
			fillectx();
			debug("encrypting string ", encrypt_in);
			int len;
			int ciphertextlen;
			unsigned char* encrypt_in_buf = new unsigned char[encrypt_in.size() + (AES_BLOCK_SIZE - encrypt_in.size() % AES_BLOCK_SIZE)];
			debug("writing encrypted bytes to ciphertext");
			if (1 != EVP_EncryptUpdate(ectx, encrypt_in_buf, &len, (const unsigned char*)encrypt_in.c_str(), encrypt_in.size())) handleErrors();
			ciphertextlen = len;
			debug("writing final bytes to ciphertext");
			if (1 != EVP_EncryptFinal_ex(ectx, encrypt_in_buf + ciphertextlen, &len)) handleErrors();
			ciphertextlen += len;
			debug("free ctx obj");
			EVP_CIPHER_CTX_free(ectx);
			for (int i = 0; i < ciphertextlen; ++i)
			{
				encrypt_out.push_back(encrypt_in_buf[i]);
			}
			debug("free encrypt_in_buf");
			debug("encrypted string ", encrypt_out);
			debug("encrypt good");
			delete[] encrypt_in_buf;
			return ciphertextlen;
		}

		int decrypt(std::string& ciphertext, std::string& plainstr, unsigned char *aad = 0, int aad_len = 0)
		{
			debug("decrypt called");
			filldctx();
			debug("decrypting string ", ciphertext);
			unsigned char* buf = new unsigned char[ciphertext.size()];
			debug("writing unencrypted bytes to plaintext");
			int len;
			if (!EVP_DecryptUpdate(dctx, buf, &len, (const unsigned char*)ciphertext.c_str(), ciphertext.size())) handleErrors();
			debug("writing final bytes to plaintext");
			int oldlen = len;
			int ret = EVP_DecryptFinal_ex(dctx, buf + oldlen, &len);
			oldlen += len;
			debug("freeing ctx obj");
			EVP_CIPHER_CTX_free(dctx);
			for (int i = 0; i < oldlen; ++i)
			{
				plainstr.push_back(buf[i]);
			}
			debug("the decrypted string ", plainstr);
			debug("freeing ciphertext buf");
			delete[] buf;
			if (ret == 1)
			{
				debug("decryption good");
				return oldlen;
			}
			else
			{
				debug("EVP Decryption Error! check != -1");
				debug("decryption failed, but the program could be configured to work anyway, sometimes the decrypted data comes out fine but it throws errors anyway, probably because something is coming up as insecure");
				return -1;
			}
		}

		std::string keyToString()
		{
			std::string newstr;
			for (int i = 0; i < 32; ++i)
			{
				newstr.push_back(key[i]);
			}
			return newstr;
		}

		std::string ivToString()
		{
			std::string newstr;
			for (int i = 0; i < 16; ++i)
			{
				newstr.push_back(iv[i]);
			}
			return newstr;
		}

		void keyFromString(std::string _key)
		{
			for (int i = 0; i < 32; ++i)
			{
				key[i] = _key[i];
			}
		}

		void ivFromString(std::string _iv)
		{
			for (int i = 0; i < 16; ++i)
			{
				iv[i] = _iv[i];
			}
		}
	};

	std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e)
	{
		debug("writing cryptinfo key and iv to stream");
		debug("writing key");
		for (int i = 0; i < 32; i++)
		{
			os << e.key[i];
		}
		debug("key written");
		debug("writing iv");
		for (int i = 0; i < AES_BLOCK_SIZE; i++)
		{
			os << e.iv[i];
		}
		debug("iv written");
		debug("writing cryptinfo to stream good");
		return os;
	}

	char* operator<<(char* os, cryptinfo  &e)
	{
		debug("writing cryptinfo key and iv to buffer");
		debug("writing key");
		for (int i = 0; i < 32; i++)
		{
			os[i] = e.key[i];
		}
		debug("key written");
		debug("writing iv");
		for (int i = 32, j = 0; i < 32 + AES_BLOCK_SIZE; ++i, ++j)
		{
			os[i] = e.iv[j];
		}
		debug("iv written");
		debug("writing cryptinfo to buffer good");
		return os;
	}

	std::istream& operator>>(std::istream &os, cryptinfo  &e)
	{
		debug("reading key and iv from stream into cryptinfo");
		debug("reading key");
		for (int i = 0; i < 32; i++)
		{
			os >> e.key[i];
		}
		debug("key read");
		debug("reading iv");
		for (int i = 0; i < AES_BLOCK_SIZE; i++)
		{
			os >> e.iv[i];
		}
		debug("iv read");
		debug("reading key and iv from stream good");
		return os;
	}

	char* operator>>(char* os, cryptinfo  &e)
	{
		debug("reading key and iv from buffer into cryptinfo");
		debug("reading key");
		for (int i = 0; i < 32; i++)
		{
			e.key[i] = os[i];
		}
		debug("key read");
		debug("reading iv");
		for (int i = 32; i < 32 + AES_BLOCK_SIZE; i++)
		{
			e.iv[i] = os[i];
		}
		debug("iv read");
		debug("reading key and iv from buffer good");
		return os;
	}
};
