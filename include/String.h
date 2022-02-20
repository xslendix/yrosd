#pragma once

#include <Common.h>

// Stolen from https://stackoverflow.com/a/217605

static inline void ltrim(string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(),
        s.end());
}

// trim from both ends (in place)
static inline void trim(string& s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline string ltrim_copy(string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline string rtrim_copy(string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}