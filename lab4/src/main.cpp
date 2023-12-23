#include <iostream>

#include <boost/program_options.hpp>

#include "utils.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  auto count = 0;

  po::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")(
      "count", po::value<int>(&count)->default_value(5),
      "set philosopers count");

  po::variables_map vars_map;
  po::store(po::parse_command_line(argc, argv, desc), vars_map);
  po::notify(vars_map);

  if (vars_map.count("help")) {
    std::cout << desc << '\n';
    return 0;
  }

  std::cout << "Set philosophers count to " << count << '\n';
  utils::Perform(count);
}
