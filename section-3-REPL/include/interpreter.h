#pragma once
#include <vector>
#include <unordered_map>
#include "parser.h"

class IRInterpreter {
    public:
        void execute(const std::vector<IR>& ir);
    private:
        std::unordered_map<std::string, int> variables;
    };
    