.include "m2560def.inc"
.list

.CSEG
.org 0x0000

ldi r16, low(RAMEND)
out spl, r16
ldi r16, high(RAMEND)
out sph, r16

ldi r16, 0b11111111
out DDRA, r16

ldi r16, 0b11111111
out DDRF, r16

ldi r16, 0b00001000
sts DDRK, r16

ldi r16, 0b00101000
sts PORTL, r16

.def timerReg = r19

.def ledA = r21
.def ledF = r22
ldi ledF, 0b10000000

.def counterAH = r23
ldi counterAH, 0b10000000
.def counterAL = r24
ldi counterAL, 0b00000001
.def counterF = r25
ldi counterF, 0b10000000


start:
	lds r16, PINL
	sbrs r16, 3
    rcall ledAswitch
	lds r16, PINL
	sbrs r16, 5
	rcall ledFswitch
	rjmp start


ledAswitch: ; algo3
	rcall timer4
	ldi ledA, 0b00000000
	or ledA, counterAH
	or ledA, counterAL
	out PORTA, ledA
	lsr counterAH
	lsl counterAL
	cpi counterAH, 0b00001000
	brne ledAswitch
	rjmp resetA

resetA:
	rcall timer4
	ldi ledA, 0b00000000
	out PORTA, ledA
	ldi counterAH, 0b10000000
	ldi counterAL, 0b00000001
	ret

ledFswitch: ; algo6
	rcall timer4
	out PORTF, ledF
	lsr ledF
	lsr ledF
	lsr counterF
	cpi counterF, 0b00001000
	breq setFcontinue
	cpi counterF, 0b00000000
	brne ledFswitch
	rjmp resetF

setFcontinue:
	ldi ledF, 0b01000000
	rjmp ledFswitch

resetF:
	rcall timer4
	out PORTF, ledF
	ldi ledF, 0b10000000
	ldi counterF, 0b10000000
	ret

timer4:
	.equ freq = 58500 ; 0,9 sec
	// (16/8) * 64 * 10^(-6) * (2^16 - 58500)
	ldi timerReg, (freq >> 8)
	sts TCNT4H, timerReg
	ldi timerReg, (freq & 0x00FF)
	sts TCNT4L, timerReg

	ldi timerReg, 0b00000000
	sts TCCR4A, timerReg
	ldi timerReg, 0b00000101 ; OVF 1024
	sts TCCR4B, timerReg
	rcall ovf4
	ret

ovf4:
	in timerReg, TIFR4
	sbrs timerReg, TOV4
	rjmp ovf4

	ldi timerReg, (1 << TOV4)
	out TIFR4, timerReg

	ldi timerReg, 0b00000000
	sts TCCR4B, timerReg
	ret

