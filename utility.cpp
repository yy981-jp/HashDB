#include <iostream>
#include <fstream>
#include <filesystem>
#include <yy981/random.h>
#include <yy981/string.h>
#include <yy981/sha256.h>

#include "def.h"
#include "utility.h"

namespace fs = std::filesystem;


std::string recovery(std::string str) {
	st::replace(str,"\\n","\n");
	st::replace(str,"\\t","\t");
	return str;
}

std::string tostr(uint8_t type) {
	switch (type) {
		case md::hsTPlain: return "Text";
		case md::hsTHash: return "Hash";
		case md::hsFile: return "File";
		case md::hsDir: return "Dir";
		default: throw std::runtime_error("tostr(): md-hs分岐エラー");
	}
	return "ERROR";
}

bool isHash(const std::string& str) {
    return str.size() == 64 &&
           std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isxdigit(c); });
}

std::string getHash(const std::string& str) {
	if (isHash(str)) {
		hsType = md::hsTHash;
		return str;
	}
	if (!fs::exists(str)) {
		hsType = md::hsTPlain;
		return sha256(str);
	}
	if (fs::is_regular_file(str)) {
		hsType = md::hsFile;
		return sha256f(str);
	}
	if (fs::is_directory(str)) {
		hsType = md::hsDir;
		std::string hash;
		std::string opath = (fs::path(str) / fs::path(dirHashName)).string();
		if (!fs::exists(opath)) {
			hash = sha256(randomSeed());
			std::ofstream(opath) << hash;
		} else std::getline(std::ifstream(opath),hash);
		return hash;
	}
	throw std::runtime_error("getHash(): hsType分岐エラー");

	return "0000000000000000000000000000000000000000000000000000000000000000";
}

std::string join_from(size_t start) {
	std::ostringstream oss;
	for (size_t i = start; i < input.size(); ++i) {
		if (i != start) oss << " ";
		oss << input[i];
	}
	return oss.str();
}
