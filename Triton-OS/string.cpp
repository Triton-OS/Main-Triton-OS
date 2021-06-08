#include "string.h"
#include "memory.h"

char Buffer[128];

void ClearBuffer() {
	for (int i = 0; i < 128; i++) Buffer[i] = '\0';
	//memset(&Buffer, '\0', 128);
}

const char* to_string(uint64_t value) {
	uint8_t size = 0;

	// Zählen
	for (uint64_t i = value; i / 10 > 0; i /= 10)
		size++;

	uint8_t i;

	for(i = 0; value / 10 > 0; value /= 10, i++)
		Buffer[size - i] = value % 10 + '0';

	Buffer[size - i] = value % 10 + '0';
	Buffer[size + 1] = '\0';
	
	return Buffer;
}

const char* to_string(int64_t value) {

	if (value > 0) return to_string((uint64_t)value);
	else {

		Buffer[0] = '-';
		value *= -1;
		uint8_t size = 0;

		// Zählen
		for (uint64_t i = value; i / 10 > 0; i /= 10)
			size++;

		uint8_t i;

		for (i = 0; value / 10 > 0; value /= 10, i++)
			Buffer[size - i + 1] = value % 10 + '0';

		Buffer[size - i + 1] = value % 10 + '0';
		Buffer[size + 2] = '\0';

		return Buffer;
	}
}

const char* to_string(double value, uint8_t decimalPlaces) {
	char* intPtr = (char*)to_string((int64_t)value);
	char* doublePtr = Buffer;

	if (decimalPlaces > 20) decimalPlaces = 20;
	if (value < 0) value *= -1;

	while (*intPtr != 0)
	{
		*doublePtr = *intPtr;
		intPtr++;
		doublePtr++;
	}
	
	*doublePtr = '.';
	doublePtr++;

	double nValue = value - (int)value;

	for (uint8_t i = 0; i < decimalPlaces; i++) {
		nValue *= 10;
		*doublePtr = (int)nValue + '0';
		nValue -= (int)nValue;
		doublePtr++;
	}
	
	*doublePtr = '\0';

	return Buffer;
}

const char* to_hexString(uint64_t value) {
	uint64_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t tmp;
	uint8_t size = 8 * 2 - 1;

	ClearBuffer();

	for (uint8_t i = 0; i < size; i++) {
		ptr = ((uint8_t*)valPtr + i);
		tmp = ((*ptr & 0xF0) >> 4);
		Buffer[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
		tmp = (*ptr & 0x0F);
		Buffer[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
	}

	Buffer[size + 1] = 0;

	return Buffer;
}

const char* to_hexString(uint32_t value) {
	uint32_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t tmp;
	uint8_t size = 4 * 2 - 1;

	ClearBuffer();

	for (uint8_t i = 0; i < size; i++) {
		ptr = ((uint8_t*)valPtr + i);
		tmp = ((*ptr & 0xF0) >> 4);
		Buffer[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
		tmp = (*ptr & 0x0F);
		Buffer[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
	}

	Buffer[size + 1] = 0;

	return Buffer;
}

const char* to_hexString(uint16_t value) {
	uint16_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t tmp;
	uint8_t size = 2 * 2 - 1;

	ClearBuffer();

	for (uint8_t i = 0; i < size; i++) {
		ptr = ((uint8_t*)valPtr + i);
		tmp = ((*ptr & 0xF0) >> 4);
		Buffer[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
		tmp = (*ptr & 0x0F);
		Buffer[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
	}

	Buffer[size + 1] = 0;

	return Buffer;
}

const char* to_hexString(uint8_t value) {
	uint8_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t tmp;
	uint8_t size = 1 * 2 - 1;

	ClearBuffer();

	for (uint8_t i = 0; i < size; i++) {
		ptr = ((uint8_t*)valPtr + i);
		tmp = ((*ptr & 0xF0) >> 4);
		Buffer[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
		tmp = (*ptr & 0x0F);
		Buffer[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
	}

	Buffer[size + 1] = 0;

	return Buffer;
}

uint16_t get_length_of(const char* value) {
	uint16_t index = 0;
	while (value[index] != '\0') index++;
	return index;
}

/*
string::string() {

	delete[] str;
	str = new char[1];
	this->str[0] = '\0';
	this->size = 0;

	return;
}

string::string(const char* value) {

	// Counting the size of the string
	for (this->size = 0; value[this->size] != '\0'; this->size++);
	
	this->str = new char[this->size];

	// filling the string
	for (uint16_t i = 0; i < this->size; i++) str[i] = value[i];
	//this->str[this->size] = '\0';

	return;
}

string::string(char value[]) {

	// Counting the size of the string
	for (this->size = 0; value[this->size] != '\0'; this->size++);

	delete[] this->str;
	str = new char[this->size];

	// filling the string
	for (uint16_t i = this->size - 1; i != 0; i--) str[i] = value[i];

	return;
}

string::string(char value) {
	delete[] str;
	this->str = new char[2];
	this->str[0] = value;
	this->str[1] = '\0';
	this->size = 1;

	return;
}

string::~string() {

	delete[] this->str;

	return;
}

uint16_t string::length() {
	return this->size;
}

const char* string::c_str() {
	return this->str;
}
*/