#pragma once
#include <GoldenAge/Debug.h>

struct toongraphics {
	unsigned int id_head;
	unsigned int id_shoulder;
	unsigned int id_gloves;
	unsigned int id_chest;
	unsigned int id_legs;
	unsigned int id_boots;

private:
	friend char* operator<<(char* os, const toongraphics& e);
	friend char* operator>>(char* os, const toongraphics& e);
	friend std::ostream& operator<<(std::ostream &os, const toongraphics& e);
	friend std::istream& operator>>(std::istream &os, toongraphics  &e);
};

std::ostream& operator<<(std::ostream &os, const  toongraphics  &e)
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

char* operator<<(char* os, const  toongraphics  &e)
{
	debug("writing toongraphics to buffer");
	int len = 0;
	int last_len = len;
	memcpy(os + last_len, &e.id_head, (len += sizeof(e.id_head)));
	int last_len = len;
	memcpy(os + last_len, &e.id_shoulder, (len += sizeof(e.id_shoulder)));
	int last_len = len;
	memcpy(os + last_len, &e.id_gloves, (len += sizeof(e.id_gloves)));
	int last_len = len;
	memcpy(os + last_len, &e.id_chest, (len += sizeof(e.id_chest)));
	int last_len = len;
	memcpy(os + last_len, &e.id_legs, (len += sizeof(e.id_legs)));
	int last_len = len;
	memcpy(os + last_len, &e.id_boots, (len += sizeof(e.id_boots)));
	*(os + len + 1) = '\0';
	debug("data written: ", os);
	return os;
}

std::istream& operator>>(std::istream &os, toongraphics  &e)
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

char* operator>>(char* os, toongraphics  &e)
{
	debug("reading in toongraphics from buffer");
	int len = 0;
	int last_len = len;
	memcpy(&e.id_head, os + last_len, (len += sizeof(e.id_head)));
	int last_len = len;
	debug("head: ", e.id_head);
	memcpy(&e.id_shoulder, os + last_len, (len += sizeof(e.id_shoulder)));
	int last_len = len;
	debug("shoulder: ", e.id_shoulder);
	memcpy(&e.id_gloves, os + last_len, (len += sizeof(e.id_gloves)));
	int last_len = len;
	debug("gloves: ", e.id_gloves);
	memcpy(&e.id_chest, os + last_len, (len += sizeof(e.id_chest)));
	int last_len = len;
	debug("chest: ", e.id_chest);
	memcpy(&e.id_legs, os + last_len, (len += sizeof(e.id_legs)));
	int last_len = len;
	debug("legs: ", e.id_legs);
	memcpy(&e.id_boots, os + last_len, (len += sizeof(e.id_boots)));
	debug("boots: ", e.id_boots);
	debug("done reading in toongraphics");
	return os;
}