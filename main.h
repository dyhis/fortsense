#include <Windows.h>
#include <iostream>
#include <consoleapi3.h>
#include <regex>
#include "render.h"
#include "mapper.h"
#include "prot.h"
HWND windowid = NULL;
int choice;
int choice1;

bool deleteFile(const std::string& filename) {
	if (std::remove(filename.c_str()) != 0) {
		SPOOF_FUNC
			// Error occurred while deleting the file
			return false;
	}
	return true;
}


void slowPrint(const std::string& text, std::chrono::milliseconds delay) {
	for (char c : text) {
		std::cout << c << std::flush;
		std::this_thread::sleep_for(delay);
	}
}

void sleepMilliseconds(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void DeleteKey(std::ifstream& File)
{

	std::string regfile("key.txt");
	std::ofstream(regfile, std::ios::trunc);
	File.setstate(std::ios::failbit);
	remove(regfile.c_str());
}

bool fileExists(const std::string& filename) {
	std::ifstream file(filename.c_str());
	return file.good();
}

std::string readFileIntoString(const std::string& path) {

	auto ss = std::ostringstream{};
	std::ifstream input_file(path);
	if (!input_file.is_open()) {
		std::cerr << SK("Could Not Open License Key File") << std::endl;
		exit(EXIT_FAILURE);
	}
	ss << input_file.rdbuf();
	return ss.str();
}

