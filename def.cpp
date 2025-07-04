#include <yy981/env.h>
#include "def.h"


const std::string localappdata = getEnv("localappdata");
const std::string path = localappdata + "//yy981//hashdb.umb";
const std::string ppath = localappdata + "//yy981//hashdb.dat";
const std::string dirHashName = ".HashDB";
const std::string EM0 = "0への干渉は出来ません";
const std::string sysPass = "66730fbd55d74c1cdec9b89e1c895217f3bc7f4e19b416a3068ec111fba143e3";
uint8_t hsType;
std::vector<std::string> input;
