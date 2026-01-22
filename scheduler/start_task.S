.global start_task

start_task:
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
  mov r30, r20
  mov r31, r21
  ; Store the low byte of the old stack in the address pointed by Z, then increment Z.
  in r16, 0x3D
  ST Z+, r16

  ; Store the high byte of the old stack in the address pointed by the Z.
  in r16, 0x3E
  ST Z, r16

  ; Change stack.
  out 0x3D, r24    ; SPL
  out 0x3E, r25    ; SPH
  clr r0            ; r1 = 0
  ; ldi r16, 0x80     ; I = 1
  ; out 0x3F, r16     ; SREG
  sei 
  mov r30, r22      ; Z = task address
  mov r31, r23
  ijmp