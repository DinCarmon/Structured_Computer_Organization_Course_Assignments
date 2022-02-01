Main:
	limm $sp, $zero, $zero, 2048
	limm $s0, $zero, $zero, 1 #constant 1
	limm $s1, $zero, $zero, 4096 #an address of a temporary help array
	limm $a0, $zero, $zero, 1024 #start address
	limm $a1, $zero, $zero, 1039 #end address
	jal $zero, $zero, $zero, MergeSort

	halt $zero, $zero, $zero, 0




MergeSort: #$a0 is the start address, $a1 is the end address
	bne $zero, $a0, $a1, GoOn #if ($a0=$a1)
	jr $ra, $zero, $zero, 0 #end MergeSort

GoOn:
	sub $sp, $sp, $s0, 0 #$sp--
	sw $ra, $sp, $zero, 0 #save $ra for later
	sub $sp, $sp, $s0, 0 #$sp--
	sw $a0, $sp, $zero, 0 #save $a0 for later
	sub $sp, $sp, $s0, 0 #$sp--
	sw $a1, $sp, $zero, 0 #save $a1 for later
	sub $sp, $sp, $s0, 0 #$sp--
	sw $t0, $sp, $zero, 0 #save $t0 for later
	sub $sp, $sp, $s0, 0 #$sp--
	sw $t1, $sp, $zero, 0 #save $t1 for later
	sub $sp, $sp, $s0, 0 #$sp--
	sw $t2, $sp, $zero, 0 #save $t2 for later

	add $t0, $a0, $a1, 0 #$t0=$a0+$a1
	sra $t0, $t0, $s0, 0 #$t0=($a0+$a1)/2
	add $t1, $t0, $s0, 0 #$t1=($a0+$a1)/2 + 1
	add $t2, $a1, $zero, 0 #$t2 is temporary for help
	add $a1, $t0, $zero, 0
	jal $zero, $zero, $zero, MergeSort
	add $a1, $t2, $zero, 0
	add $t2, $a0, $zero, 0
	add $a0, $t1, $zero, 0
	jal $zero, $zero, $zero, MergeSort
	add $a0, $t2, $zero, 0
	
	add $t2, $s1, $zero, 0 # an address of a temporary array
StartLoop1:
	bgt $zero, $a0, $t0, EndLoop1
	bgt $zero, $t1, $a1, EndLoop1
	lw $t3, $a0, $zero, 0 #$t3=mem[$a0]
	lw $v0, $t1, $zero, 0 #$v0=mem[$t1]
	bgt $zero, $t3, $v0, caseB #if(mem[$a0]<=mem[$t1])
	sw $v0, $t2, $zero, 0 #copy to temporary
	add $t1, $t1, $s0, 0 #$t1++
	beq $zero, $zero, $zero, Continue
caseB: #else
	sw $t3, $t2, $zero, 0 #copy to temporary
	add $a0, $a0, $s0, 0 #$t1++
Continue:
	add $t2, $t2, $s0, 0 #$t2++
	beq $zero, $zero, $zero, StartLoop1

EndLoop1:
	bgt $zero, $a0, $t0, EndLoop2
	lw $t3, $a0, $zero, 0 #copying to temporary
	sw $t3, $t2, $zero, 0
	add $a0, $a0, $s0, 0 #$a0++
	add $t2, $t2, $s0, 0 #$t2++
	beq $zero, $zero, $zero, EndLoop1

EndLoop2:
	bgt $zero, $t1, $a1, EndLoop3
	lw $t3, $t1, $zero, 0 #copying to temporary
	sw $t3, $t2, $zero, 0
	add $t1, $t1, $s0, 0 #$t1++
	add $t2, $t2, $s0, 0 #$t2++
	beq $zero, $zero, $zero, EndLoop2

EndLoop3: #copy the temporary array
	lw $a0, $sp, $zero, 4 #$a0 is the start address again
	add $t2, $s1, $zero, 0 # $t2 is again an address of a temporary array
StartLoop4:
	bgt $zero, $a0, $a1, EndLoop4
	lw $t3, $t2, $zero, 0 #copying from temporary
	sw $t3, $a0, $zero, 0
	add $t2, $t2, $s0, 0 #$t2++
	add $a0, $a0, $s0, 0 #$a0++
	beq $zero, $zero, $zero, StartLoop4

EndLoop4:
	lw $t2, $sp, $zero, 0 #upload $t2 for later
	add $sp, $sp, $s0, 0 #$sp++
	lw $t1, $sp, $zero, 0 #upload $t1 for later
	add $sp, $sp, $s0, 0 #$sp++
	lw $t0, $sp, $zero, 0 #upload $t0 for later
	add $sp, $sp, $s0, 0 #$sp++
	lw $a1, $sp, $zero, 0 #upload $a1 for later
	add $sp, $sp, $s0, 0 #$sp++
	lw $a0, $sp, $zero, 0 #upload $a0 for later
	add $sp, $sp, $s0, 0 #$sp++
	lw $ra, $sp, $zero, 0 #upload $ra for later
	add $sp, $sp, $s0, 0 #$sp++
	jr $ra, $zero, $zero, 0 #return




.word 1024 1
.word 1025 7
.word 1026 8
.word 1027 3
.word 1028 12
.word 1029 5
.word 1030 10
.word 1031 2
.word 1032 6
.word 1033 11
.word 1034 13
.word 1035 15
.word 1036 4
.word 1037 14
.word 1038 9
.word 1039 16


