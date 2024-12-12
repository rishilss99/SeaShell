#pragma once

#include "comm.h"

void exitFn(std::stringstream &ss);

void echoFn(std::stringstream &ss);

void typeFn(std::stringstream &ss);

void pwdFn(std::stringstream &ss);

void cdFn(std::stringstream &ss);

bool checkIfSupportedCommand(std::string &command);

void executeCommand(std::string &input);