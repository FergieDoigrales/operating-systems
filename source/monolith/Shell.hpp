#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>
#include <vector>

void parseCommand(const std::string &command, std::vector<std::string> &args);
void fergieShell();

#endif 