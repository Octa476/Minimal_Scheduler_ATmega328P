.global start_first_task

start_first_task:
  out 0x3D, r24    ; SPL
  out 0x3E, r25    ; SPH
  clr r0           ; r1 = 0
  ; "ldi r16, 0x80\n\t"     ; I = 1
  ; "out 0x3F, r16\n\t"     ; SREG
  sei
  mov r30, r22      ; Z = task address
  mov r31, r23
  ijmp