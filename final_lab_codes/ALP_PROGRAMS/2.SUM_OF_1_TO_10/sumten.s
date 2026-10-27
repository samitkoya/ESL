 
  THUMB
  AREA sum,CODE,READONLY
  EXPORT DVALUE
  EXPORT sumten
  IMPORT globvar
ENTRY

sumten
  MOV R1,#10		; load 10 to regester
  MOV R2,#0			; empty the register to store result
loop
  ADD R2,R2,R1		; add the content of R1 with result at R2
  SUBS R1,#0x01		; Decreament R1 by 1
  BNE loop			; repeat till r1 goes 0

  LDR R0,=globvar	; load the addr of var into R0
  STR R2,[R0]		; store the result into addrs location
  
  BX LR				; jumps back to C code

  AREA DATA1,DATA,READWRITE
DVALUE
  DCD 0x00000000;


  ALIGN
  END
		


  