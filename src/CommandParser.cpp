#include <CommandParser.h>

#include <Common.h>

static unique_ptr<CommandParser> s_the = nullptr;

CommandParser& CommandParser::the()
{
    if (!s_the) {
        unique_ptr<CommandParser> new_the(std::make_unique<CommandParser>());
        s_the = std::move(new_the);
    }
    return *s_the;
}

void parse(string& command)
{
}
