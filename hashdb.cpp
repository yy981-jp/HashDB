#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <sstream>
#include <yy981/return.h>
#include <yy981/string.h>
#include <yy981/umb.h>
#include <yy981/sha256.h>

namespace fs = std::filesystem;

namespace md {
	constexpr uint8_t
		error(0x00), read(0x01), write(0x02), del(0x03), cal(0x04),
		create(0x14), edit(0x15),
		hsTPlain(0x21), hsTHash(0x22), hsFile(0x23);
}

namespace flag {
	constexpr uint8_t hide(0x51), enc(0x52);
}

const std::string localappdata = std::string(std::getenv("localappdata"));
const std::string path = localappdata + "\\yy981\\hashdb.umb";
const std::string ppath = localappdata + "\\yy981\\hashdb.dat";
uint8_t hsType;
std::vector<std::string> input;
constexpr std::string explanation = 
	"Hash-DataBase v1.0.1   (C) 2025 yy981"
	"\nUsage: hashdb <Mode> <Key(ハッシュ値orファイルパス)> [<内容>]"
	"\n\n   <Mode>"
	"\n read\t要素を読み込み\t\trとも記述可能"
	"\n edit\t要素を編集or作成\teとも記述可能"
	"\n del\t要素を削除\t\tdやdeleteとも記述可能"
	"\n cal\tSha256計算を行う\tcとも記述可能"
	"\n pass\t管理パスワードを変更"
	"\n view\t一覧を表示"
	"\n 省略し、内容が入力されなかった場合、要素を読み込む"
	"\n 　　　内容が入力された場合、要素を書き込む"
	"\n\n   <内容>"
	"\n editモード\t保存したい情報を入力"
	"\n モード\t保存したい情報を入力"
	"\n その他\t必要なし"
	"\n   <応答文について>"
	"\n []内はハッシュ値 ()内はハッシュ値の元のデータ"
	"\n Hash\t入力文字列をそのままHash値と解釈"
	"\n Text\t入力文字列をハッシュ計算し解釈"
	"\n File\t入力文字列をパスと解釈し、そのファイルをハッシュ計算し解釈"
	"\n\n   <構文例>"
	"\n read example"
	"\n example"
	"\n edit example text"
	"\n example text"
	"\n del example"
	"\n cal example"
	"\n pass password"
	"\n view password";


inline void rs() {
	std::cerr << "SYSTEM: 該当データが存在しません\n";
	exit(1);
}

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
	} else {
		if (!fs::exists(str)) {
			hsType = md::hsTPlain;
			return sha256(str);
		}
		hsType = md::hsFile;
		return sha256f(str);
	}
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


int main(ARGC) {
	if (argc < 2 || argc > 4) return_u(explanation,true);
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
		if (st::isor(input[1],"edit","e")) {
			if (argc < 4) return_e("内容が存在しません",2);
			mode = md::write;
			contents = join_from(3);
		}
		else if (st::isor(input[1],"delete","del","d")) mode = md::del;
		else if (st::isor(input[1],"read","r")) mode = md::read;
		else if (st::isor(input[1],"cal","c")) mode = md::cal;
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
