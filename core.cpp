#include <stdexcept>
#include <fstream>
#include <filesystem>

#include <yy981/sha256.h>

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
		return str;
	}
	if (!fs::exists(str)) {
		hsType = md::hsTPlain;
		return sha256(str);
	}
	if (fs::is_regular_file(str)) {
		if (fs::file_size(str)==64 && !isHash(str)) {
			std::ifstream ifs(str);
			if (!ifs) throw std::runtime_error("getHash()::hsTextHash: ファイルを開けませんでした");
			std::string hash;
			std::getline(ifs,hash);
			hsType = md::hsTextHash;
			return hash;
		}
		hsType = md::hsFile;
		return sha256f(str);
	}
	if (fs::is_directory(str)) {
		hsType = md::hsDir;
		std::string hash;
		std::string DHPath = (fs::path(str) / fs::path(dirHashName)).string();
		if (!fs::exists(DHPath)) {
			hash = sha256(randomSeed());
			writeDirHashDB(DHPath,hash);
		} else hash = loadDirHashDB(DHPath);
		return hash;
	}
	throw std::runtime_error("getHash(): hsType分岐エラー");

	return "0000000000000000000000000000000000000000000000000000000000000000";
}
