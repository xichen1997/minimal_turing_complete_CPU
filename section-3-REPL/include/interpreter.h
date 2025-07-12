#pragma once
#include <vector>
#include <unordered_map>
#include "parser.h"
#include <iostream>
#include <sstream>

class IRInterpreter {
    public:
        void execute(const IR& inst);
        void execute(const std::vector<IR>& ir);
        void execute(const std::vector<IR>& ir, const std::unordered_map<std::string, size_t>& labelMap);
    private:
        std::unordered_map<std::string, int> variables;
};