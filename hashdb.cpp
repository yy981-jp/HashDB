#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <sstream>
#include <yy981/return.h>
#include <yy981/string.h>
#include <yy981/umb.h>
#include <yy981/sha256.h>

#include "def.h"
#include "utility.h"

namespace fs = std::filesystem;


int main(int argc, char *argv[]) {
	if (argc < 2) return_u(explanation,true);
	uint8_t mode;
	std::string hash, contents, pass;

// ロード
	if (!fs::exists(path) || !fs::exists(ppath)) {
		if (!fs::exists(path)) if (!std::ofstream(path)) return_e("初期セットアップ失敗_UMB",3);
		if (!fs::exists(ppath)) {
			std::cout << "パスワードを設定: ";
			std::string input;
			std::cin >> input;
			std::ofstream ofs(ppath);
			if (!ofs) return_e("初期セットアップ失敗_DAT",3);
			ofs << getHash(input);
		}
		std::cout << "初期セットアップ完了\n";
		return 0;
	}
	std::unordered_map<std::string,std::string> db = UMB::load(path);

// 入力整形
	input = st::charV(argc,argv);
	bool hash_i2 = true;
	if (argc==2) {
		// 読み込み - 省略
		mode = md::read;
		hash = getHash(input[1]);
	} else {
		if (is_or(input[1],"edit","e")) {
			if (argc < 4) return_e("内容が存在しません",2);
			mode = md::write;
			contents = join_from(3);
		}
		else if (is_or(input[1],"delete","del","d")) mode = md::del;
		else if (is_or(input[1],"read","r")) mode = md::read;
		else if (is_or(input[1],"cal","c")) mode = md::cal;
		else if (input[1]=="view") {
			std::ifstream ifs(ppath);
			std::getline(ifs,pass);
			if (getHash(input[2])==pass) {
				for (const auto& [key, value] : db) {
					std::cout << key << "> " << value << std::endl;
				}
				return 0;
			} else return_e("認証失敗",981);
		}
		else if (input[1]=="pass") {
			std::ifstream ifs(ppath);
			std::getline(ifs,pass);
			if (getHash(input[2])==pass) {
				std::ofstream ofs(ppath);
				if (!ofs) return_e("出力ファイルが開けません");
				std::cout << "新しいパスワードを入力: ";
				std::string input_tmp;
				std::cin >> input_tmp;
				ofs << getHash(input_tmp);
				return 0;
			} else return_e("認証失敗",981);
		} else {
			hash_i2 = false;
			mode = md::write;
			hash = getHash(input[1]);
			contents = join_from(2);
		}
		// 省略に対する処理
		if (hash_i2) hash = getHash(input[2]);
	}
	if (mode == md::write) {if (db.contains(hash)) mode = md::edit; else mode = md::create;}

// 処理
	switch (mode) {
		case md::read: {
			if (!db.contains(hash)) rs();
			std::cout << "[" << hash << "](" << tostr(hsType) << ")\n";
			std::cout << recovery(db[hash]) << std::endl;
		} break;
		case md::del: {
			if (!db.contains(hash)) rs();
			db.erase(hash);
			UMB::save(db,path);
			std::cout << "[" << hash << "](" << tostr(hsType) << ")を削除しました\n";
		} break;
		case md::create: {
			if (contents.empty()) return_e("内容が存在しません",2);
			db.emplace(hash,contents);
			UMB::save(db,path);
			std::cout << "[" << hash << "](" << tostr(hsType) << ")を作成しました\n";
		} break;
		case md::edit: {
			if (contents.empty()) return_e("内容が存在しません",2);
			db[hash] = contents;
			UMB::save(db,path);
			std::cout << "[" << hash << "](" << tostr(hsType) << ")を更新しました\n";
		} break;
		case md::cal: std::cout << "Sha256Hash計算結果: [" << hash << "](" << tostr(hsType) << ")\n"; break;
		default: throw std::runtime_error("内部モード分岐後エラー");
	}
}
