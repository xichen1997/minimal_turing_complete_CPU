#pragma once
#include <vector>
#include <unordered_map>
#include "parser.h"
#include <iostream>
#include <sstream>

class IRInterpreter {
    public:
        IRInterpreter();
        void executeSingleInstruction(const IR& inst);
        void execute(const std::vector<IR>& ir);
        void execute(const std::vector<IR>& ir, const std::unordered_map<std::string, size_t>& labelMap);
    private:
        std::unordered_map<std::string, int> variables;
        std::unordered_map<std::string, std::pair<uint16_t, size_t>> arrayMap;
        std::unordered_map<std::string, uint16_t> addressMap;
        std::vector<int> memory;
        uint16_t nextAddress;
        
        uint16_t allocate(const std::string& name);
        int resolve(const std::string& operand);
};