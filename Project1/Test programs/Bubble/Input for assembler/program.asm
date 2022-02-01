Main:
	limm $sp, $zero, $zero, 2048
	limm $s0, $zero, $zero, 1 #constant 1
	limm $s1, $zero, $zero, 1038 #constant (end address-1)
	limm $t0, $zero, $zero, 1038

StartOutLoop:
	limm $a0, $zero, $zero, 1024 #start address
	bgt $zero, $a0, $t0, EndOutLoop

StartInLoop:
	bgt $zero, $a0, $t0, EndInLoop
	jal $zero, $zero, $zero, Bubble
	add $a0, $a0, $s0, 0 #$a0++
	beq $zero, $zero, $zero, StartInLoop
EndInLoop:

	sub $t0, $t0, $s0 ,0 #$t0--
	beq $zero, $zero, $zero, StartOutLoop
EndOutLoop:

	halt $zero, $zero, $zero, 0




Bubble: #$a0= address which should be compared to the one next to it.
	sub $sp, $sp, $s0, 0 #$sp--
	sw $a0, $sp, $zero, 0 #keep $a0 for later

	add $a1, $a0, $s0, 0 #$a1=$a0+1
	lw $t1, $a0, $zero, 0 #$t1=mem[$a0]
	lw $t2, $a1, $zero, 0 #$t2=mem[$a1]
	bgt $zero, $t1, $t2, GoOn
	sw $t2, $a0, $zero, 0 #mem[$a0]=$t2
	sw $t1, $a1, $zero, 0 #mem[$a1]=$t1
GoOn:

	lw $a0, $sp, $zero, 0 #keep $a0 for later
	add $sp, $sp, $s0, 0 #$sp++

	jr $ra, $zero, $zero, 0




.word 1024 7
.word 1025 6
.word 1026 12
.word 1027 1
.word 1028 11
.word 1029 5
.word 1030 8
.word 1031 3
.word 1032 16
.word 1033 2
.word 1034 10
.word 1035 13
.word 1036 15
.word 1037 4
.word 1038 9
.word 1039 14