#include <yy981/env.h>
#include "def.h"


const std::string localappdata = getEnv("localappdata");
const std::string path = localappdata + "//yy981//hashdb.umb";
const std::string ppath = localappdata + "//yy981//hashdb.dat";
const std::string dirHashName = ".HashDB";
uint8_t hsType;
std::vector<std::string> input;
