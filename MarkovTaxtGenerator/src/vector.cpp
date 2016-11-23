#include "vector.h"



int Wrap(int value, int max) {
	max++;
	return (value + max) % max;
}

// You could also take an existing vector as a parameter.
vector<std::string> split(std::string value, vector<std::string> delimiters, int minSnippetLength) {
	vector<std::string> result;

	while (true) {
		auto size = (int)value.size();
		auto closestDistance = size;

		for (auto& delimiter : delimiters) {
			auto pos = value.find(delimiter);

			if (closestDistance > pos) {
				closestDistance = pos;
			}
		}

		if (closestDistance > (int)value.size() || (int)value.size() == 0) {
			return result;
		}

		auto snippet = value.substr(0, closestDistance);
		if ((int)snippet.size() >= minSnippetLength)
			result.push_back(snippet);

		if ((int)value.size() > closestDistance + 1) {
			value = value.substr(closestDistance + 1);

		}
		else {
			result.push_back(value);
			return result;
		}
	}
}

std::string join(const vector<std::string>& value, std::string joiner) {
	{
		std::stringstream ss;
		for (size_t i = 0; i < value.size(); ++i)
		{
			if (i != 0)
				ss << joiner;
			ss << value[i];
		}
		return ss.str();

	}
}
