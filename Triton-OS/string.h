#pragma once

#include <stdint.h>
#include "dynamic.h"

const char* to_string(uint64_t value);
const char* to_string(int64_t value);
const char* to_string(double value, uint8_t decimalPlaces);
const char* to_hexString(uint64_t value);
const char* to_hexString(uint32_t value);
const char* to_hexString(uint16_t value);
const char* to_hexString(uint8_t value);
uint16_t get_length_of(const char* value);

/*
class string {

public:

	//string();
	string(const char* value);
	//string(char value[]);
	//string(char value);

	~string();

	uint16_t length();
	const char* c_str();

	string operator= (const char* value) { return string(value); }
	string operator= (char value[]) { return string(value); }
	string operator= (int64_t value[]) { return string(to_string((uint64_t)value)); }
	string operator= (uint64_t value[]) { return string(to_string((uint64_t)value)); }
	string operator= (int32_t value[]) { return string(to_string((uint64_t)value)); }
	string operator= (uint32_t value[]) { return string(to_string((uint64_t)value)); }
	string operator= (int16_t value[]) { return string(to_string((uint64_t)value)); }
	string operator= (uint16_t value[]) { return string(to_string((uint64_t)value)); }
	//string operator= (char value) { return string(); }

private:

	char* str = new char[0];
	uint16_t size = 0;
};
*/