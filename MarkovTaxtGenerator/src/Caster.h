#pragma once
#include "vector.h";
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <unordered_set>
#include <typeinfo>
#include <memory>
#include <typeindex>

using std::unordered_map;

struct BaseContainer {
	std::type_index typeID;
};

template <typename T>
struct Container : BaseContainer {
	Container<T>(T value);

	T value;
};

class Caster
{
public:
	template<typename T>
	static Container<T>* Caster::Try(BaseContainer* value);

	template<typename T>
	static bool Is(BaseContainer* value);
};


template<typename T>
Container<T>* Caster::Try(BaseContainer* value) {
	if (Is<T>(value))
		return (Container<T>*)(value);
	else
		return nullptr;
}


template<typename T>
bool Caster::Is(BaseContainer* value) {
	return std::type_index(typeid(T)).hash_code() == value->typeID.hash_code();
}

template<typename T>
Container<T>::Container(T value) : value(value), BaseContainer{ typeid(T) } {

}
