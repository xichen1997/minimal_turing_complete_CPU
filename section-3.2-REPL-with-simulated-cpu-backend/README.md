# REPL with simulated cpu as backend

## Current architecture

We launch the REPL to compile the .dsl to IR but not the machine code. Thus, only the interpreter can run these IR, which is not what we finally want, and we want all of the software run based on their compiled machine code.


## Changes in this section

### add .runfromCPU

This command will use codegen to generate machine code and load the program with start location 0x2000. And the data section will be start from 0x8000 as before.

### simulate the bootloader

```c++
    }else if(cmd == ".runfromCPU"){
        std::string filename; 
        uint16_t addr = 0x2000;
        iss >> filename; // Extract filename from the stream
        if(filename.empty()) {
            std::cout << "Error: Please provide a filename after .runfromCPU" << std::endl;
            return;
        }
        std::ifstream file(filename);
        if(!file.is_open()){
            std::cout << "Error: File not found: " << filename << std::endl;
            return;
        }
        file.close();
        
        Codegen gen(filename);
        gen.writeToHex("output.bin", "output.hex");
        gen.writeToFile("output.asm");
        std::vector<uint8_t> code = gen.getCode();
        DEBUG_PRINT("Code size: " << code.size());
        // load the code to the CPU
        MinimalCPU cpu;
        DEBUG_PRINT("Loading code to CPU...");
        DEBUG_PRINT("Code size: " << code.size());
        cpu.loadProgram(code, addr);
        cpu.run();
    }
```


### now REPL can serve as an interpreter, which execute IR, but also could use .runfromCPU to run real machine code from CPU level.