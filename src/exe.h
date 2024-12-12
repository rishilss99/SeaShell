#pragma once

#include "comm.h"

void processPath();

void forkAndExec(std::string &path, std::stringstream &ss);

bool checkIfPathExecutable(std::string &executable);

void executeExecutable(std::string &command);