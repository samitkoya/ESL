
  AREA  MULTI , CODE , READONLY  
  EXPORT MULTIPLY
  IMPORT globvar

MULTIPLY

 	LDR R0,=0X706F   	 
 	LDR R1,=0X7161  
  	MUL R2,R1,R0 
	
 	 
 	MOV R3,R2;
	LDR R3,=globvar	; load the addr of var into R0
  	STR R2,[R3]	
	NOP
	NOP
 	BX LR				; jumps back to C code

  END
		

