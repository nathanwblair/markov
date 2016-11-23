#pragma once
#include "vector.h"
#include <list>
using std::list;
typedef unsigned int uint;

#include <iostream>
#include <iomanip>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include "Caster.h"


class Lookup :
	public vector<uint> {
public:
	Lookup();
	~Lookup();
	vector<BaseContainer*> decoder;


	int AddHash(uint value);
	int FindHash(uint value);

	template <typename T>
	int Add(const T value);
	template <typename T>
	int Find(const T value);

	template <typename T>
	vector<int> Convert(const vector<T>& values);

	template<typename T>
	vector<T> Extract(const vector<int>& values, const T & tDefault);

	template<typename T>
	vector<T> Extract(const T & tDefault);

	template <typename T>
	T* Try(int index);

	template <typename T>
	bool Is(int index);

	template <typename T>
	T Get(int index, const T& tDefault);
};

template<typename T>
int Lookup::Add(const T value) {
	auto prevSize = size();
	auto hash = std::hash<T>()(value);

	auto result = AddHash(hash);

	if (result == prevSize) {
		decoder.push_back((BaseContainer*)(new Container<T>{ T(value) }));
	}

	return result;
}

template<typename T>
int Lookup::Find(const T value) {
	return FindHash(std::hash<T>()(value));
}

template<typename T>
vector<int> Lookup::Convert(const vector<T>& values) {
	auto size = (int)values.size();
	vector<int> result(size);

	for (int index = 0; index < size; index++) {
		result[index] = Add(values[index]);
	}

	return result;
}

template<typename T>
vector<T> Lookup::Extract(const vector<int>& values, const T& tDefault) {
	auto size = (int)values.size();
	vector<T> result(size);

	for (int index = 0; index < size; index++) {
		result[index] = Get<T>(values[index], tDefault);
	}

	return result;
}

template<typename T>
vector<T> Lookup::Extract(const T& tDefault) {
	auto size = (int)decoder.size();
	vector<T> result(size);

	for (int index = 0; index < size; index++) {
		result[index] = Get<T>(index, tDefault);
	}

	return result;
}

template<typename T>
T * Lookup::Try(int index) {
	return Caster::Try<Container<T>*>(decoder[index]);
}

template<typename T>
inline bool Lookup::Is(int index) {
	return Caster::Is<Container<T>*>(decoder[index]);
}

template<typename T>
T Lookup::Get(int index, const T & tDefault) {
	auto result = Caster::Try<T>(decoder[index]);

	if (result != NULL) {
		return result->value;
	}
	else {
		return tDefault;
	}
}
