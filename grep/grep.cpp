// grep.cpp: определяет точку входа для приложения.
//
#include "grep.h"
#include <iostream>
#include <filesystem>


int main(int argc, char* argv[]) {
//    if (argc < 2) {
        //std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
        //return 1;
    //}

    //std::filesystem::path directory = argv[1];    
    //if (!std::filesystem::is_directory(directory)) {
        //std::cerr << "Error: " << directory << " is not a directory" << std::endl;
        //return 1;
    //}
    Grep g;
    g.search("E:\\download", "59478501", {true,false,".json"});
	return 0;
}
