#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <bits/stdint-uintn.h>

#include <spdlog/spdlog.h>

typedef uint8_t u8;

const unsigned int MAJOR_VERSION = 0;
const unsigned int MINOR_VERSION = 0;
const unsigned int PATCH_VERSION = 1;

inline const std::string get_version_number() { return fmt::format("{}.{}.{}", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION); }

using std::ifstream;
using std::string;
using std::string_view;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::stringstream;
