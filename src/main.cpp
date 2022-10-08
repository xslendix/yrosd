#include <Common.h>
#include <Application.h>
#include <Config.h>
#include <Signals.h>
#include <Processes.h>

#include <cstdlib>
#include <sys/types.h>
#include <signal.h>

#include <argparse/argparse.hpp>

int
main(int argc, char* argv[])
{
#pragma region Arguments

  argparse::ArgumentParser args(argv[0], get_version_number(), argparse::default_arguments::help);

  args.add_argument("-V", "--verbose")
      .help("enable verbose output")
      .default_value(false)
      .implicit_value(true);

  args.add_argument("-t", "--test")
      .help("test configuration file")
      .default_value(false)
      .implicit_value(true);

  args.add_argument("-D", "--daemonize")
      .help("daemonize the process (NOT TESTED)")
      .default_value(false)
      .implicit_value(true);

  args.add_argument("-v", "--version")
      .help("prints version information and exits")
      .default_value(false)
      .implicit_value(true);

  args.add_argument("-r", "--reload")
      .help("reload the first yrosd process")
      .default_value(false)
      .implicit_value(true);

  args.add_argument("-s", "--stop")
      .help("stop the first yrosd process")
      .default_value(false)
      .implicit_value(true);

  try {
    args.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << args;
    std::exit(1);
  }

#pragma endregion

#pragma region Argument parsing

  if (args["reload"] == true) {
    pid_t pid = Processes::getProcIdByName("yrosd");
    kill(pid, 1);
    return EXIT_SUCCESS;
  }

  if (args["stop"] == true) {
    pid_t pid = Processes::getProcIdByName("yrosd");
    kill(pid, SIGTERM);
    return EXIT_SUCCESS;
  }

  if (args["version"] == true) {
    std::cout << get_version_number() << std::endl;
    return EXIT_SUCCESS;
  }

  if (args["verbose"] == true)
    spdlog::set_level(spdlog::level::debug);

#pragma endregion

  initialize_signals();

  Configuration::the();

  if (args["test"] == false) {
    if (args["daemonize"] == true) {
      if (fork() == 0) {
        App::the().Run();
      }
    } else {
      App::the().Run();
    }
  }

  return EXIT_SUCCESS;
}
