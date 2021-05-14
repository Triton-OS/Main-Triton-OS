#include "Bitmap.h"

bool Bitmap::operator[](uint64_t index) {
	if (index > this->Size * 8) return false;
	uint64_t byteIndex = index / 8;
	uint8_t bitIndex = index % 8;
	uint8_t bitIndexer = 0b10000000 >> bitIndex;

	if ((this->Buffer[byteIndex] & bitIndexer) > 0) return true;
	return false;
}

bool Bitmap::Set(uint64_t index, bool value) {
	if (index > this->Size * 8) return false;
	uint64_t byteIndex = index / 8;
	uint8_t bitIndex = index % 8;
	uint8_t bitIndexer = 0b10000000 >> bitIndex;
	this->Buffer[byteIndex] &= ~bitIndexer;

	if (value) this->Buffer[byteIndex] |= bitIndexer;
	return true;
}