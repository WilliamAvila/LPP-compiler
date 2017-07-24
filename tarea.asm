li $t1, 10
addi $t2, $t1, 2
li $t1, 5
mult $t2, $t1
mflo $t3
addi $t1, $t3, -1
move $a0, $t1
li   $v0, 1
syscall

li $t3, 10
li $t5, 2
li $t4, 5
mult $t5, $t4
mflo $t6
add $t4, $t3, $t6
addi $t3, $t4, -1
move $a0, $t3
li   $v0, 1
syscall

li $t5, 10
addi $t6, $t5, 2
li $t5, 2
div $t6, $t5
mflo $t7
li $t5, 5
mult $t7, $t5
mflo $t6
addi $t5, $t6, -1
move $a0, $t5
li   $v0, 1
syscall

