let i = 0;
let base = 100;      

read_line:
in ch;
let addr = base + i;
store ch -> addr;
let i = i + 1;
let max = 31;
if i <= max goto read_line;


load 36864 -> c0;         ; buffer[0]
load 36865 -> c1;         ; buffer[1]
load 36866 -> c2;         ; buffer[2]
load 36867 -> c3;         ; buffer[3]

let r = 114;              ; ASCII 'r'
let u = 117;              ; ASCII 'u'
let n = 110;              ; ASCII 'n'
let one = 49;             ; ASCII '1'
let two = 50;             ; ASCII '2'

if c0 <= r goto not_run;
if c1 <= u goto not_run;
if c2 <= n goto not_run;

if c3 <= one goto run1;
if c3 <= two goto run2;

not_run:
goto main;

run1:
goto program1;

run2:
goto program2;

main:
goto read_line;

program1:
out r;        
halt;

program2:
out u;       
halt;
