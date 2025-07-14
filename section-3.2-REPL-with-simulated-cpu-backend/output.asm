; Generated assembly code
; Code size: 24 bytes

; IR Instructions:
; IR[0]: STORE_CONST 2 -> __temp__0
; IR[1]: STORE __temp__0 -> a
; IR[2]: OUT a
; IR[3]: HALT

; Machine Code:
0000: 0x02 0x03 0x01 ; LOAD_CONST (LOAD Rd, const) R3, 1
0003: 0x04 0x80 0x00 0x02 ; STORE_CONST (STORE addr, const) 0x8000, 2
0007: 0x01 0x00 0x80 0x00 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8000
000b: 0x03 0x80 0x01 0x00 ; STORE (STORE addr, Rs) 0x8001, R0
000f: 0x01 0x00 0x80 0x01 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8001
0013: 0x03 0xff 0x00 0x00 ; STORE (STORE addr, Rs) 0xff00, R0
0017: 0x00 ; HALT (HALT)
