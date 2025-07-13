#include "interpreter.h"
#include <iostream>
#include <stdexcept>

void IRInterpreter::executeSingleInstruction(const IR& inst){
    if (inst.op == OpCode::LOAD_CONST) {
        variables[inst.result] = std::stoi(inst.arg1);
    } else if (inst.op == OpCode::LOAD_VAR) {
        if (variables.find(inst.arg1) == variables.end()) {
            throw std::runtime_error("Undefined variable: " + inst.arg1);
        }
        variables[inst.result] = variables[inst.arg1];
    } else if (inst.op == OpCode::ADD) {
        if (variables.find(inst.arg1) == variables.end()) {
            throw std::runtime_error("Undefined variable: " + inst.arg1);
        }
        if (variables.find(inst.arg2) == variables.end()) {
            throw std::runtime_error("Undefined variable: " + inst.arg2);
        }
        variables[inst.result] = variables[inst.arg1] + variables[inst.arg2];
    } else if (inst.op == OpCode::SUB) {
        if (variables.find(inst.arg1) == variables.end()) {
            throw std::runtime_error("Undefined variable: " + inst.arg1);
        }
        if (variables.find(inst.arg2) == variables.end()) {
            throw std::runtime_error("Undefined variable: " + inst.arg2);
        }
        variables[inst.result] = variables[inst.arg1] - variables[inst.arg2];
    } else if (inst.op == OpCode::STORE) {
        if (variables.find(inst.arg1) == variables.end()) {
            throw std::runtime_error("Undefined variable: " + inst.arg1);
        }
        variables[inst.result] = variables[inst.arg1];
    } else if (inst.op == OpCode::STORE_CONST) {
        variables[inst.result] = std::stoi(inst.arg1);
    } else if (inst.op == OpCode::OUT) {
        if (variables.find(inst.arg1) == variables.end()) {
            throw std::runtime_error("Undefined variable: " + inst.arg1);
        }
        std::cout << variables[inst.arg1] << std::endl;
    } else if (inst.op == OpCode::HALT) {
        throw std::runtime_error("HALT instruction executed");
    } else if (inst.op == OpCode::LABEL) {
        return;
    }
    return;
}

void IRInterpreter::execute(const std::vector<IR>& ir) {
    for (const auto& inst : ir) {
        if (inst.op == OpCode::LOAD_CONST) {
            variables[inst.result] = std::stoi(inst.arg1);
        } else if (inst.op == OpCode::LOAD_VAR) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            variables[inst.result] = variables[inst.arg1];
        } else if (inst.op == OpCode::ADD) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            if (variables.find(inst.arg2) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg2);
            }
            variables[inst.result] = variables[inst.arg1] + variables[inst.arg2];
        } else if (inst.op == OpCode::SUB) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            if (variables.find(inst.arg2) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg2);
            }
            variables[inst.result] = variables[inst.arg1] - variables[inst.arg2];
        } else if (inst.op == OpCode::STORE) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            variables[inst.result] = variables[inst.arg1];
        } else if (inst.op == OpCode::STORE_CONST) {
            variables[inst.result] = std::stoi(inst.arg1);
        } else if (inst.op == OpCode::OUT) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            std::cout << variables[inst.arg1] << std::endl;
        } else if (inst.op == OpCode::HALT) {
            throw std::runtime_error("HALT instruction executed");
        }
    }
}

void IRInterpreter::execute(const std::vector<IR>& ir, const std::unordered_map<std::string, size_t>& labelMap) {
    if(ir.empty()){
        return;
    }
    size_t pc = 0;
    while(pc < ir.size()){
        const auto& inst = ir[pc];
        if(inst.op == OpCode::GOTO){
            pc = labelMap.at(inst.arg1);
        }else if(inst.op == OpCode::IFLEQ){
            if(variables[inst.arg1] <= variables[inst.arg2]){
                pc = labelMap.at(inst.result);
            }
        }else{
            executeSingleInstruction(inst);
        }
        pc++;
    }
    return;
}