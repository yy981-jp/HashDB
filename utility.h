#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <yy981/random.h>


inline void rs() {
	std::cerr << "SYSTEM: 該当データが存在しません\n";
	exit(1);
}
extern void writeDirHashDB(const std::string& DHPath, const std::string& hash);
extern std::string loadDirHashDB(const std::string& DHPath);
extern std::string recovery(std::string str);
extern bool isHash(const std::string& str);
extern std::string join_from(size_t start);
