ADDI R1,R0,#-30000
ADDI R2,R0,#-1
ADDI R3,R0,#-3856
ADDI R4,R0,#-256
ADDI R5,R0,#255
ADDI R6,R0,#3855
SLL R7,R1,#5
SLL R17,R4,#3
SLL R27,R2,#31
SLL R8,R6,#0
SRL R18,R2,#3
SRL R28,R0,#7
SRA R9,R3,#4
SRA R19,R6,#2
ADD R29,R1,R6
ADD R7,R3,R4
SUB R7,R6,R5
SUB R7,R6,R1
MUL R7,R5,R6
MUL R7,R3,R5
AND R7,R4,R6
AND R7,R5,R3
OR R7,R4,R6
OR R7,R1,R3
NOR R7,R1,R2
NOR R7,R3,R4
SLT R7,R2,R3
SLT R7,R1,R5
ANDI R7,R2,#15
ANDI R7,R6,#65535
BREAK 

