This implementation is pretty simple, we just need to write a DSL program, which has the already programmed jobs:

```
:start:
    in cmd;
    if cmd <= 1 goto cmd1;
    if cmd <= 2 goto cmd2;
    goto start;

:cmd1:
    out 100;
    goto start;

:cmd2:
    in x;
    out x;
    goto start;
```

Use `.runfromCPU kernel.dsl` can launch this program and give an interactive shell to user to choose two program cmd1 and cmd2. 

Congratulation, here is a mileston, the CPU can run a very simple operating system, but it's pretty unconvenient though(The flow is fixed can't add new program.)