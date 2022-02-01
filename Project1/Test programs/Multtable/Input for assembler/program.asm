Main:
	limm $sp, $zero, $zero, 2048 #initialization of the stack address
	limm $s0, $zero, $zero, 1024 #saving in address
	limm $s1, $zero, $zero, 9 #size of the mul table
	limm $s2, $zero, $zero, 1 #constant 1

	limm $a0, $zero, $zero, 0 #initialization of i-1
OutLoop:
	bgt $zero, $a0, $s1, EndOutLoop #if i>10 out
	limm $a1, $zero, $zero, 0 #initialization of j-1

InLoop:
	bgt $zero, $a1, $s1, EndInLoop #if j>10 out
	jal $zero, $zero, $zero, Mul
	add $t0, $zero, $v0, 0 #$t0=i*j
	jal $zero, $zero, $zero, PrintAddress
	sw $t0, $v0, $zero, 0 #mem[1024+(j-1)*10+i-1]=i*j
	add $a1, $a1,  $s2, 0 #j++
	beq $zero, $zero, $zero, InLoop
EndInLoop:

	add $a0, $a0,  $s2, 0 #i++
	beq $zero, $zero, $zero, OutLoop
EndOutLoop:

	halt $zero, $zero, $zero, 0




Mul: #$a0=i-1, $a1=j-1
	sub $sp, $sp, $s2, 0
	sw $a0, $sp, $zero, 0
	sub $sp, $sp, $s2, 0
	sw $a1, $sp, $zero, 0

	add $a0, $a0, $s2, 0 #$a0=i
	add $v0, $a0, $zero, 0 #$v0=i
	limm $t0, $zero, $zero, 1 #$t0=1
Loop1:
	bgt $zero, $t0, $a1, EndLoop1
	add $v0, $v0, $a0, 0
	add $t0, $t0, $s2, 0 #$t0++
	beq $zero, $zero, $zero, Loop1
EndLoop1:

	lw $a0, $sp, $zero, 0
	add $sp, $sp, $s2, 0
	lw $a1, $sp, $zero, 0
	add $sp, $sp, $s2, 0

	jr $ra, $zero, $zero, 0




PrintAddress: #$a0=i-1, $a1=j-1
	sub $sp, $sp, $s2, 0
	sw $a0, $sp, $zero, 0
	sub $sp, $sp, $s2, 0
	sw $a1, $sp, $zero, 0

	limm $t1, $zero, $zero, 10 #constant 10
	add $v0, $s0, $zero, 0 #$v0=1024
	limm $t2, $zero, $zero, 1 #$t2=1

Loop2:
	bgt $zero, $t2, $a1, EndLoop2
	add $v0, $v0, $t1, 0
	add $t2, $t2, $s2, 0 #$t2++
	beq $zero, $zero, $zero, Loop2
EndLoop2:
	limm $t2, $zero, $zero, 1 #$t2=1

Loop3:
	bgt $zero, $t2, $a0, EndLoop3
	add $v0, $v0, $s2, 0
	add $t2, $t2, $s2, 0 #$t2++
	beq $zero, $zero, $zero, Loop3
EndLoop3:

	lw $a0, $sp, $zero, 0
	add $sp, $sp, $s2, 0
	lw $a1, $sp, $zero, 0
	add $sp, $sp, $s2, 0

	jr $ra, $zero, $zero, 0