#pragma once
#include <vector>
#include <assert.h>
#include <string>
#include <sstream>
#include <iostream>

using std::vector;



template<typename T>
vector<T>& v(vector<T>& first);

template<typename T, typename... Args>
vector<T>& v(vector<T>& first, T second, Args... args);

template<typename T, typename... Args>
vector<T> v(T second, Args... args);

template<typename T>
vector<T> sub(const vector<T>& value, int start, int finish);

template<typename T>
vector<T>& addTo(vector<T>& value, const vector<T>& increment);

int Wrap(int value, int max);


template<typename T, typename... Args>
vector<T>& v(vector<T>& first, T second, Args... args) {
	first.push_back(second);

	return v(first, args...);
}

template<typename T, typename T2, typename... Args>
vector<T>& v(vector<T>& first, T second, T2 third, Args... args) {
	first.push_back(second);

	return v(first, T(third), args...);
}

template<typename T, typename... Args>
vector<T> v(T first, Args... args) {
	vector<T> result;
	return v(result, first, args...);
}

template<typename T_Result, typename T, typename... Args>
vector<T_Result> v(T first, Args... args) {
	vector<T_Result> result;
	return v(result, T_Result(first), args...);
}

template<typename T>
vector<T>& v(vector<T>& first) {
	return first;
}

template<typename T>
vector<T> sub(const vector<T>& value, int start, int finish) {
	auto max = (int)value.size();

	if (start == 0 && finish == max)
		return value;

	max++;
	start = (start + max) % max;
	finish = (finish + max) % max;
	max--;

	assert(start >= 0 && finish > 0 && start < finish && start < (max) && finish <= max);

	vector<int>::const_iterator start_pos = value.begin() + start;
	vector<int>::const_iterator finish_pos = value.begin() + finish;
	vector<int> new_arr(start_pos, finish_pos);

	return new_arr;
}

template<typename T>
vector<T>& addTo(vector<T>& value, const vector<T>& increment) {
	for (int index = 0; index < (int)value.size(); index++) {
		value[index] += increment[index];
	}

	return value;
}

// You could also take an existing vector as a parameter.
vector<std::string> split(std::string value, vector<std::string> delimiters, int minSnippetLength = 1);

std::string join(const vector<std::string>& value, std::string joiner);