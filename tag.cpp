#include <bitset>
#include <string>

#include <yy981/return.h>


std::bitset<8> parse(const std::string& text) {
	if (!text.starts_with("`")) return 0;
	size_t pos = text.find(" ");
	if (pos == std::string::npos || pos > 8) return_e("tag.cpp::parse(): `は見つかりましたが、終端(空白)が見つかりません",5);
	std::string tag = text.substr(pos);
	tag.contains()
}