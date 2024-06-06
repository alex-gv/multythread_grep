#pragma once

#include "lock_free_queue.h"
#include "is_binary.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <thread>

#include <fstream>
#include <regex>



struct Options {
    bool ignore_binaries{true};
    bool verbose_mode{false};
    std::string file_mask;
};

class Grep {
public:
    Grep() = default;
    ~Grep() {
        stop();
    }
    void stop() {
        is_file_processing_finished_ = true;
        is_processing_finished_ = true;
    };
	void search(const std::string& path, const std::string& query, const Options& opt) {        
        threads_.emplace_back(&Grep::process_directory, this, path, query, opt);
        for (auto i = 0; i < std::thread::hardware_concurrency(); ++i)
            threads_.emplace_back(&Grep::process_files, this, query, opt);
        
        std::thread print_thread(&Grep::print, this);

        for (auto& thread : threads_)
            thread.join();

         is_file_processing_finished_ = true;

         print_thread.join();
	}
private:    
    void print() {        
        std::string text;
        while (!out_queue_.is_empty() || !is_file_processing_finished_) {
            if (!out_queue_.pop_front(text)) continue;
            std::cout << text << std::endl;
        }        
    }

	void process_directory(const std::filesystem::path& path, const std::string& query, const Options& opt) {   
        try {            
		    for (const auto& entry : std::filesystem::directory_iterator(path)) {
                try {
                    if (entry.is_regular_file() &&
                        (opt.file_mask.empty() || std::regex_search(entry.path().filename().string(), std::regex(opt.file_mask)))
                        ) {
                        in_queue_.push_back(entry.path().string());
                    }
                    if (entry.is_directory())
                        process_directory(entry.path(), query, opt);                    
                }
                catch (const std::exception& ex) {
                    if (opt.verbose_mode) std::cerr << ex.what() << std::endl;
                }
            }
		} catch (const std::exception& ex) {
            if (opt.verbose_mode) std::cerr << ex.what() << std::endl;
        }
        is_processing_finished_ = true;
	}
    void process_files(const std::string& query, const Options& opt) {
        const std::regex pattern(query);
        std::string file;
        while (!in_queue_.is_empty() || !is_processing_finished_) {
            try {
                if (!in_queue_.pop_front(file)) continue;
                std::ifstream f(file.c_str());
                if (!f.is_open()) {
                    if (opt.verbose_mode) std::cout << "can't open file:" << file << std::endl;
                    continue;
                }
                std::string line;
                uint64_t line_number{ 1 };
                bool is_first{ true };
                while (std::getline(f, line)) {
                    if (opt.ignore_binaries && is_first && is_binary(line.c_str())) {
                        if (opt.verbose_mode) out_queue_.push_back(file + ": is binary");
                        break;
                    }
                    std::smatch matches;
                    std::string::const_iterator searchStart(line.cbegin());
                    while (std::regex_search(searchStart, line.cend(), matches, pattern)) {
                        out_queue_.push_back(std::to_string(line_number) + ":" + file + ":" +
                            matches.prefix().str() + "\033[31m" + matches.str() + "\033[0m" + matches.suffix().str() // green color for marking
                        );
                        searchStart = matches.suffix().first;
                    }
                    ++line_number;
                    if (is_first) is_first = false;
                }
            } catch (const std::exception& ex) {
                std::cerr << ex.what() << std::endl;
            }
        }
    }

private:	
    LockFreeQueue<std::string> in_queue_;
    LockFreeQueue<std::string> out_queue_;
    std::atomic<bool> is_file_processing_finished_{false};
    std::atomic<bool> is_processing_finished_{false};
    std::vector<std::thread> threads_;
};

