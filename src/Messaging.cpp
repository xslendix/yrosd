#include <Messaging.h>

#include <Common.h>
#include <filesystem>

namespace Messaging {

void SendMessage(string const& message)
{
    auto file = ofstream(std::filesystem::temp_directory_path() / "yros_comm");
    file << message;
    file.close();
}

}
