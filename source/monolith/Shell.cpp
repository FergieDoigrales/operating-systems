#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <time.h>
#include "Shell.hpp"

void parseCommand(const std::string &command, std::vector<std::string> &args) {
    std::istringstream iss(command);
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
}

void fergieShell() {
    std::string command;
    std::vector<std::string> args;
    char *argv[100];

    while (true) {
        std::cout << "fergieShell> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        args.clear();
        parseCommand(command, args);

        for (size_t i = 0; i < args.size(); ++i) {
            argv[i] = const_cast<char *>(args[i].c_str());
        }
        argv[args.size()] = nullptr;

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        pid_t pid = fork();
        if (pid == 0) {
            execvp(argv[0], argv);
            std::cerr << "Error: command not found" << std::endl;
            exit(1);
        } else if (pid > 0) {
            wait(nullptr);
            clock_gettime(CLOCK_MONOTONIC, &end);
            double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            std::cout << "Time taken: " << elapsed << " seconds" << std::endl;
        } else {
            std::cerr << "Error: fork failed" << std::endl;
        }
    }
}