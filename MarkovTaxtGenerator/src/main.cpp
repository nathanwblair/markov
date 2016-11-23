#pragma comment(linker, "/STACK:100000000")
#pragma comment(linker, "/HEAP:1000000000")

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "Lookup.h"
#include <random>
#include <string>
#include <algorithm>
#include <thread>
#include <iterator>
using std::string;
using Eigen::MatrixXd;
using Eigen::VectorXd;
#include <chrono>

float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck? 
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
										   //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

void call_from_thread() {
	std::cout << "Hello, World" << std::endl;
	
}
float get_random(float min, float max)
{
	static std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<> dis(min, max);
	return dis(rng);
}

string GetInput(string message) {
	printf(message.c_str());
	string response;
	std::getline(std::cin, response);

	return response;
}

void Write(string filename, string value) {
	std::ofstream file;
	file.open(filename);
	file << value.c_str();
	file.close();
}

string Read(string filename) {
	{
		string line;
		std::ifstream input(filename);
		if (input.is_open())
		{
			std::stringstream strStream;
			strStream << input.rdbuf();//read the file
			return strStream.str();//str holds the content of the file
		}
		else
		{
			return "";
		}
	}
}

void MakeLower(string& value) {
	std::transform(value.begin(), value.end(), value.begin(), ::tolower);
}

int main()
{
	auto inputFile = GetInput("Please enter full filename to train chain from : \n");
	auto outputFile = GetInput("Please enter full filename set output to : \n");

	auto defaults = split(Read("default.ini"), v<string>("\n"));
	bool usedDefaults = false;

	if (inputFile == "") {
		inputFile = defaults[0];
		bool usedDefaults = true;
	}
	if (outputFile == "") {
		outputFile = defaults[1];
		bool usedDefaults = true;
	}

	if (!usedDefaults)
		Write("default.ini", inputFile + "\n" + outputFile);

	string trainingText = "";



	{
		string line;
		std::ifstream input(inputFile);
		if (input.is_open())
		{
			std::stringstream strStream;
			strStream << input.rdbuf();//read the file
			trainingText = strStream.str();//str holds the content of the file
		}
		else {
			return 0;
		}
	}
	auto words = split(trainingText, v<string>(" ", "	 ", ".", ",", "(", ")", ";", "\"", "-", "!", ":", "?"));
	


	MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;
	Lookup lookup;
	auto result = lookup.Convert(words);
	auto resultSize = (int)result.size();
	auto lookupSize = (int)lookup.size();

	MatrixXd data = MatrixXd::Zero(lookupSize, lookupSize);

	for (int index = 0; index < resultSize; index++) {
		for (int subIndex = std::max(0, index - 5); subIndex < std::min(resultSize, index + 30); subIndex++) {
			double weight = 5;
			if (subIndex < index)
				weight = 0.2;
			else if (subIndex == index)
				continue;

			//auto change = weight / pow(index - subIndex, 2);
			data(result[subIndex], result[index]) += weight / pow(abs(index - subIndex), 1.2);
		//	data(result[subIndex], result[index]) = std::max(0.0, data(result[subIndex], result[index]));
		}
	}

	for (int index = 0; index < data.cols(); index++) {
		data.col(index) /= data.col(index).sum();
	}

	VectorXd input = VectorXd::Zero(lookupSize);
	input[0] = 1;

	vector<int> textOutput(500);

	string finalResult = "";
	for (int i = 0; i < 500; i++) {
		input = data * input;
		//auto max = input[0];
		//max = input[0];
		//auto maxIndex = 0;

		//for (int index = 1; index < lookupSize; index++) {
		//	maxIndex = input[index] > max ? index : maxIndex;

		//	if (maxIndex == index) {
		//		max = input[index];
		//	}
		//}

		float potSize = (float)input.sum();
		auto random = (double)get_random(0, potSize);

		int index = 0;
		for (double cursor = 0; cursor + input[index] < random; cursor += input[index]) {
			index++;
		}
		
		input[index] = 1;

		textOutput[i] = index;
		finalResult = join(lookup.Extract<string>(textOutput, ""), " ");
		std::cout << finalResult + "\n";
	}


	Write(outputFile, finalResult);

	int i = 0;
}