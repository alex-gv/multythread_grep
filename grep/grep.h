#pragma once

#include "lock_free_queue.h"
#include <filesystem>
#include <string>
#include <thread>
#include <vector>


/**
 * @brief The Options struct is used to define the search options.
 * The struct contains the following fields:
 * - ignore_binaries: A boolean flag indicating whether to ignore binary files.
 * - verbose_mode: A boolean flag indicating whether to enable verbose mode.
 * - file_mask: A string representing the file mask for filtering files.
 */
struct Options {
  bool ignore_binaries{true};
  bool verbose_mode{false};
  std::string file_mask;
};

/**
 * @brief The Grep class is responsible for the text search functionality.
 * It implements the search functionality for a given directory path and a query
 * string. It uses multiple threads to search for the query string in multiple
 * files simultaneously. The output is presented in a thread-safe manner using
 * LockFreeQueue.
 */
class Grep {
public:
  Grep() = default;
  ~Grep();
  // Stops the threads
  void stop();
  /**
   * @brief Searches for the query string in the given directory path.
   * @param path The directory path to search.
   * @param query The query string to search for.
   * @param opt The options for the search.
   */
  void search(const std::string &path, const std::string &query,
              const Options &opt);

private:
  // Prints the search results
  void print();
  /**
   * @brief Processes a directory recursively to find files to search.
   * @param path The directory path to process.
   * @param query The query string to search for.
   * @param opt The options for the search.
   */
  void process_directory(const std::filesystem::path &path,
                         const std::string &query, const Options &opt);
  /**
   * @brief Processes files to search for the query string.
   * @param query The query string to search for.
   * @param opt The options for the search.
   */
  void process_files(const std::string &query, const Options &opt);

private:
  // Input queue for file paths.
  LockFreeQueue<std::string> in_queue_;
  // Output queue for search results.
  LockFreeQueue<std::string> out_queue_;
  // Flag to indicate file processing is finished.
  std::atomic<bool> is_file_processing_finished_{false};
  // Flag to indicate the processing all directories is finished.
  std::atomic<bool> is_processing_finished_{false};
  // Thread pool for searching.
  std::vector<std::thread> threads_;
};
