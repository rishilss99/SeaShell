#pragma once

#include "comm.h"

void processPath();

void forkAndExec(std::string &path, std::stringstream &ss);

bool checkIfPathExecutable(std::string &executable);

bool checkIfQuotedExecutable(std::string &input);

void executeExecutable(std::string &command);

void executeQuotedExecutable(std::string &command);