#pragma once

#include <bits/stdint-uintn.h>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <vector>

#include <spdlog/spdlog.h>

typedef uint8_t u8;

const unsigned int MAJOR_VERSION = 0;
const unsigned int MINOR_VERSION = 0;
const unsigned int PATCH_VERSION = 1;

inline const std::string get_version_number() { return fmt::format("{}.{}.{}", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION); }

using std::ifstream;
using std::ofstream;
using std::shared_ptr;
using std::string;
using std::string_view;
using std::stringstream;
using std::unique_ptr;
using std::vector;

template<typename T>
static T max(T x, T y)
{
    return x > y ? x : y;
}

template<typename T>
static T min(T x, T y)
{
    return x < y ? x : y;
}
