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