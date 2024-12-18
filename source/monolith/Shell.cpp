#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <time.h>
#include <sched.h>
#include <cstring>
#include "Shell.hpp"

#define STACK_SIZE (1024 * 1024) 

void parseCommand(const std::string &command, std::vector<std::string> &args) {
    std::istringstream iss(command);
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
}

int childFunc(void *arg) {
    char **argv = static_cast<char **>(arg);
    execvp(argv[0], argv);
    std::cerr << "Error: command not found" << std::endl;
    return 1;
}

void fergieShell() {
    std::string command;
    std::vector<std::string> args;
    char *argv[100];
    char *stack = new char[STACK_SIZE]; 
    char *stackTop = stack + STACK_SIZE; 

    while (true) {
        std::cout << "fergieShell> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        if (std::cin.eof()) {
            std::cout << "\nExiting shell..." << std::endl;
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

        pid_t pid = clone(childFunc, stackTop, SIGCHLD, argv);
        if (pid == -1) {
            std::cerr << "Error: clone failed" << std::endl;
            delete[] stack;
            continue;
        }

        waitpid(pid, nullptr, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        std::cout << "Time taken: " << elapsed << " seconds" << std::endl;
    }

    delete[] stack; 
}