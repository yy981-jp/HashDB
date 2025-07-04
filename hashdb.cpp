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
#include "core.h"

namespace fs = std::filesystem;


int main(int argc, char *argv[]) {
	if (argc < 2) return_u(explanation,true);
	uint8_t mode;
	std::string hash, contents;

// ロード
	bool setup0 = false;
	if (!fs::exists(path) || !fs::exists(ppath)) {
		if (!fs::exists(path)) if (!std::ofstream(path)) return_e("初期セットアップ失敗_UMB",3);
		if (!fs::exists(ppath)) {
			std::cout << "パスワードを設定: ";
			std::string input;
			std::cin >> input;
			std::ofstream ofs(ppath);
			if (!ofs) return_e("初期セットアップ失敗_DAT",3);
			ofs << getHash(input);
			setup0 = true;
		}
		std::cout << "初期セットアップ完了\n";
		return 0;
	}
	std::unordered_map<std::string,std::string> db = UMB::load(path);
	if (setup0) {
		db.emplace(sysPass,sha256(randomSeed()));
		UMB::save(db,path);
	}

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
		else if (is_or(input[1],"show","s","cal","c")) mode = md::show;
		else if (is_or(input[1],"dir","h")) {
			hash_i2 = false;
			mode = md::dir;
			hash = getHash(input[3]);
		}
		else if (input[1]=="view") {
			std::string pass;
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
			std::string pass;
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
		// 省略しなかったものに対する処理
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
		case md::show: std::cout << "Hash: [" << hash << "](" << tostr(hsType) << ")\n"; break;
		case md::dir: {
			if (!fs::exists(input[2])) return_e("コマンドライン引数2が示すパスは存在しません",4); 
			std::string DHPath = (fs::path(input[2]) / fs::path(dirHashName)).string();
			if (!fs::exists(DHPath)) writeDirHashDB(DHPath,hash); else {
				std::cout << "[警告] そのディレクトリに.HashDBは既に存在します 上書きしますか? [Y,N]?";
				std::string input_tmp;
				std::cin >> input_tmp;
				if (is_or(input_tmp,"Y","y","yes")) {
					std::cout << " 変更前: [" << loadDirHashDB(DHPath) << "]\n";
					writeDirHashDB(DHPath,hash);
					std::cout << " 変更後: [" << hash << "](" << tostr(hsType) << ")\n";
				} else std::cout << "操作を中止しました\n";
			}
		} break;
		default: throw std::runtime_error("内部モード分岐後エラー");
	}
}
