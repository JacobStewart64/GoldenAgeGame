#pragma once
#include <GoldenAge/debug.h>

namespace ga {
	struct toongraphics {
		unsigned int id_head;
		unsigned int id_shoulder;
		unsigned int id_gloves;
		unsigned int id_chest;
		unsigned int id_legs;
		unsigned int id_boots;

	std::string to_string()
	{
		std::string tgstr;
		pushIntBytes(id_head, tgstr);
		pushIntBytes(id_shoulder, tgstr);
		pushIntBytes(id_gloves, tgstr);
		pushIntBytes(id_chest, tgstr);
		pushIntBytes(id_legs, tgstr);
		pushIntBytes(id_boots, tgstr);
		return tgstr;
	}

	void from_string(std::string& str)
	{
		debug("from str ", str);
		id_head = (unsigned int)str[0];
		id_shoulder = (unsigned int)str[4];
		id_gloves = (unsigned int)str[8];
		id_chest = (unsigned int)str[12];
		id_legs = (unsigned int)str[16];
		id_boots = (unsigned int)str[20];
	}

	private:

		void pushIntBytes(unsigned int num, std::string& str)
		{
			debug("push int bytes ", str);
			char buf[4];
			memcpy(buf, &num, 4);
			str.push_back(buf[0]);
			str.push_back(buf[1]);
			str.push_back(buf[2]);
			str.push_back(buf[3]);
			debug("push int bytes ", str);
		}

		friend char* operator<<(char* os, const toongraphics& e);
		friend char* operator>>(char* os, const toongraphics& e);
		friend std::ostream& operator<<(std::ostream &os, const toongraphics& e);
		friend std::istream& operator>>(std::istream &os, toongraphics  &e);
	};


	std::ostream& operator<<(std::ostream &os, const ga::toongraphics  &e)
	{
		debug("writing toongraphics to stream");
		os << e.id_head << "\n";
		os << e.id_shoulder << "\n";
		os << e.id_gloves << "\n";
		os << e.id_chest << "\n";
		os << e.id_legs << "\n";
		os << e.id_boots << "\n";
		return os;
	}

	char* operator<<(char* os, const ga::toongraphics  &e)
	{
		debug("writing toongraphics to buffer");
		int len = 0;
		int last_len = len;
		memcpy(os + last_len, &e.id_head, (len += sizeof(e.id_head)));
		last_len = len;
		memcpy(os + last_len, &e.id_shoulder, (len += sizeof(e.id_shoulder)));
		last_len = len;
		memcpy(os + last_len, &e.id_gloves, (len += sizeof(e.id_gloves)));
		last_len = len;
		memcpy(os + last_len, &e.id_chest, (len += sizeof(e.id_chest)));
		last_len = len;
		memcpy(os + last_len, &e.id_legs, (len += sizeof(e.id_legs)));
		last_len = len;
		memcpy(os + last_len, &e.id_boots, (len += sizeof(e.id_boots)));
		*(os + len + 1) = '\0';
		debug("data written: ", os);
		return os;
	}

	std::istream& operator>>(std::istream &os, ga::toongraphics  &e)
	{
		debug("reading toongraphics from stream");
		os >> e.id_head;
		debug("head: ", e.id_head);
		os >> e.id_shoulder;
		debug("shoulder: ", e.id_shoulder);
		os >> e.id_gloves;
		debug("gloves: ", e.id_gloves);
		os >> e.id_chest;
		debug("chest: ", e.id_chest);
		os >> e.id_legs;
		debug("legs: ", e.id_legs);
		os >> e.id_boots;
		debug("boots: ", e.id_boots);
		return os;
	}

	char* operator>>(char* os, ga::toongraphics  &e)
	{
		debug("reading in toongraphics from buffer");
		int len = 0;
		int last_len = len;
		memcpy(&e.id_head, os + last_len, (len += sizeof(e.id_head)));
		last_len = len;
		debug("head: ", e.id_head);
		memcpy(&e.id_shoulder, os + last_len, (len += sizeof(e.id_shoulder)));
		last_len = len;
		debug("shoulder: ", e.id_shoulder);
		memcpy(&e.id_gloves, os + last_len, (len += sizeof(e.id_gloves)));
		last_len = len;
		debug("gloves: ", e.id_gloves);
		memcpy(&e.id_chest, os + last_len, (len += sizeof(e.id_chest)));
		last_len = len;
		debug("chest: ", e.id_chest);
		memcpy(&e.id_legs, os + last_len, (len += sizeof(e.id_legs)));
		last_len = len;
		debug("legs: ", e.id_legs);
		memcpy(&e.id_boots, os + last_len, (len += sizeof(e.id_boots)));
		debug("boots: ", e.id_boots);
		debug("done reading in toongraphics");
		return os;
	}
};
