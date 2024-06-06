#include "grep.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

std::string getCmdOption(char **begin, char **end, const std::string &option) {
  auto itr = std::find(begin, end, option);
  if (itr != end && ++itr != end)
    return std::string(*itr);
  return {};
}

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
  return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[]) {
  Options opt;

  if (argc < 3) {
    std::cerr << "Usage: grep <directory> <search query> [options]"
              << std::endl;
    return 1;
  }

  std::filesystem::path directory = argv[1];
  if (!std::filesystem::is_directory(directory)) {
    std::cerr << "Error: " << directory << " is not a directory" << std::endl;
    return 1;
  }

  if (!argv[2]) {
    std::cerr << "Usage: grep <directory> <search query> [options]"
              << std::endl;
    return 1;
  }

  if (cmdOptionExists(argv, argv + argc, "--help") ||
      cmdOptionExists(argv, argv + argc, "-h")) {
    std::cout << "Usage: grep <directory> <search query> [options]"
              << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-m regexp mask for files (*.json, *.txt, etc...)"
              << std::endl;
    std::cout << "-v verbose mode" << std::endl;
    std::cout << "-di don't ignore bunary files" << std::endl;
  }
  if (cmdOptionExists(argv, argv + argc, "-v")) {
    opt.verbose_mode = true;
  }
  if (const auto &mask = getCmdOption(argv, argv + argc, "-m"); !mask.empty()) {
    opt.file_mask = mask;
  }
  if (cmdOptionExists(argv, argv + argc, "-di")) {
    opt.ignore_binaries = false;
  }
  Grep g;
  g.search(argv[1], argv[2], opt);
  return 0;
}
