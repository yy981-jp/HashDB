#pragma once
#include <fstream>
#include <windows.h>
#include <yy981/random.h>


inline void rs() {
	std::cerr << "SYSTEM: 該当データが存在しません\n";
	exit(1);
}
extern std::string recovery(std::string str);
extern std::string tostr(uint8_t type);
extern bool isHash(const std::string& str);
extern std::string getHash(const std::string& str);
extern std::string join_from(size_t start);
