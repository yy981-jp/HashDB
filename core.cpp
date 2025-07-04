#include <stdexcept>
#include <fstream>
#include <filesystem>

#include <yy981/sha256.h>
#include <yy981/return.h>

#include "core.h"
#include "def.h"
#include "utility.h"

namespace fs = std::filesystem;


std::string tostr(uint8_t type) {
	switch (type) {
		case md::hsTPlain: return "Text";
		case md::hsTHash: return "Hash";
		case md::hsFile: return "File";
		case md::hsDir: return "Dir";
		case md::hsTextHash: return "HashTF";
		case md::hsBinHash: return "HashBF";
		default: throw std::runtime_error("tostr(): md-hs分岐エラー");
	}
	return "ERROR";
}

std::string getHash(const std::string& str) {
	if (isHash(str)) {
		hsType = md::hsTHash;
		if (str==sysPass) return_e(EM0,6);
		return str;
	}
	if (!fs::exists(str)) {
		hsType = md::hsTPlain;
		std::string hash = sha256(str);
		if (hash==sysPass) return_e(EM0,6);
		return hash;
	}
	if (fs::is_regular_file(str)) {
		if (fs::file_size(str)==64 && !isHash(str)) {
			std::ifstream ifs(str);
			if (!ifs) throw std::runtime_error("getHash()::hsTextHash: ファイルを開けませんでした");
			std::string hash;
			std::getline(ifs,hash);
			hsType = md::hsTextHash;
			if (hash==sysPass) return_e(EM0,6);
			return hash;
		}
		hsType = md::hsFile;
		std::string hash = sha256f(str);
		if (hash==sysPass) return_e(EM0,6);
		return hash;
	}
	if (fs::is_directory(str)) {
		hsType = md::hsDir;
		std::string hash;
		std::string DHPath = (fs::path(str) / fs::path(dirHashName)).string();
		if (!fs::exists(DHPath)) {
			hash = sha256(randomSeed());
			writeDirHashDB(DHPath,hash);
		} else hash = loadDirHashDB(DHPath);
		if (hash==sysPass) return_e(EM0,6);
		return hash;
	}
	throw std::runtime_error("getHash(): hsType分岐エラー");

	return "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
}
