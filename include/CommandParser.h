#pragma once

#include <Common.h>

class CommandParser {
public:
    void parse(string &command);
    static CommandParser& the();

};
