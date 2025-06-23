#include <iostream>
#include <fstream>
#include <filesystem>
#include <yy981/random.h>
#include <yy981/string.h>
#include <yy981/sha256.h>

#include "def.h"
#include "utility.h"

namespace fs = std::filesystem;


void writeDirHashDB(const std::string& DHPath, const std::string& hash) {
	std::ofstream ofs_DHPath(DHPath);
	if (!ofs_DHPath) throw std::runtime_error("writeDirHashDB(): .HashDBファイルに出力できません");
	ofs_DHPath << hash;
}

std::string loadDirHashDB(const std::string& DHPath) {
	std::string hash;
	std::ifstream ifs_DHPath(DHPath);
	if (!ifs_DHPath) throw std::runtime_error("loadDirHashDB(): .HashDBファイルを読み込めません");
	std::getline(ifs_DHPath,hash);
	return hash;
}


std::string recovery(std::string str) {
	st::replace(str,"\\n","\n");
	st::replace(str,"\\t","\t");
	return str;
}

bool isHash(const std::string& str) {
    return str.size() == 64 && std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isxdigit(c); });
}

std::string join_from(size_t start) {
	std::ostringstream oss;
	for (size_t i = start; i < input.size(); ++i) {
		if (i != start) oss << " ";
		oss << input[i];
	}
	return oss.str();
}
