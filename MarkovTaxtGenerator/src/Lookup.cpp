#include "Lookup.h"

Lookup::Lookup()
{
	uint a;
}


Lookup::~Lookup()
{
}

int Lookup::AddHash(uint value) {
	auto result = std::find(begin(), end(), value);
	int resultIndex = std::distance(begin(), result);

	if (result == end()) {
		push_back(value);
	}
	
	return resultIndex;
}

int Lookup::FindHash(uint value) {
//	auto iter = begin();
//	auto index = 0;
//	while (iter != end()) {
//		if (value == *iter) {
//			return index;
//		}
//
//		index++;
//		iter++;
//	}
//
//	return -1;

	auto result = std::find(begin(), end(), value);
	int resultIndex = std::distance(begin(), result);

	if (result == end()) {
		return -1;
		//push_back(value);
	}

	return resultIndex;
}
