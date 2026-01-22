.global resume_task

resume_task:
  ; Push everything on the stack.
  in r0, 0x3F
  push r0 ; SREG

  push r0
  push r1
  push r2
  push r3
  push r4
  push r5
  push r6
  push r7
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
  push r16
  push r17
  push r18
  push r19
  push r20
  push r21
  push r22
  push r23
  push r24
  push r25
  push r26
  push r27
  push r28
  push r29
  push r30
  push r31
  ; Save the old stack pointer into the old_stack_pointer_addr.
  ; Prepare the Z register.
  mov r30, r24
  mov r31, r25
  ; Store the low byte of the old stack in the address pointed by Z, then increment Z.
  in r16, 0x3D
  ST Z+, r16

  ; Store the high byte of the old stack in the address pointed by the Z.
  in r16, 0x3E
  ST Z, r16

  ; Now change the stack pointer with the new one and start popping everything.
  out 0x3D, r22
  out 0x3E, r23
  
  ; Pop everything in reverse order.
  pop r31
  pop r30
  pop r29
  pop r28
  pop r27
  pop r26
  pop r25
  pop r24
  pop r23
  pop r22
  pop r21
  pop r20
  pop r19
  pop r18
  pop r17
  pop r16
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop r7
  pop r6
  pop r5
  pop r4
  pop r3
  pop r2
  pop r1
  pop r0

  pop r0
  out 0x3F, r0

  ; The execution will go back to the scheduler, from the scheduler back to the ISR and then to the task.
  ret