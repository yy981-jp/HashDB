#pragma once
#define VERSION "v1.3"
#include <string>


inline std::string explanation = 
	"Hash-DataBase " VERSION "   (C) 2025 yy981"
	"\nUsage: HS <Mode> <Key(ハッシュ値orファイルパス)> [<内容>]"
	"\n\n   <Mode>"
	"\n read\t要素を読み込み\t\trとも記述可能"
	"\n edit\t要素を編集or作成\teとも記述可能"
	"\n del\t要素を削除\t\tdやdeleteとも記述可能"
	"\n show\tHash値を求める\t\tsとも記述可能"
	"\n dir\t.HashDBを手動で生成\thとも記述可能"
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
	"\n Dir\t入力文字列をパスと解釈し、そのフォルダに配置された.HashDBを読む(存在しない場合は自動で生成)"
	"\n HashTF\tファイル内の文字列をそのままHash値と解釈"
	// "\n HashBF\tファイル内のバイナリデータをそのままHash値と解釈"
	"\n\n   <構文例>"
	"\n read example"
	"\n example"
	"\n edit example text"
	"\n example text"
	"\n del example"
	"\n show example"
	"\n dir exampleDir exampleFile"
	"\n pass password"
	"\n view password";

namespace md {
	constexpr uint8_t
		error(0x00), read(0x01), write(0x02), del(0x03), show(0x04),
		create(0x14), edit(0x15), dir(0x16),
		hsTPlain(0x21), hsTHash(0x22), hsFile(0x23), hsDir(0x24), hsTextHash(0x25), hsBinHash(0x26);
}

namespace flag {
	constexpr uint8_t hide(0x51), enc(0x52);
}

extern const std::string localappdata;
extern const std::string path;
extern const std::string ppath;
extern const std::string dirHashName;
extern uint8_t hsType;
extern std::vector<std::string> input;
