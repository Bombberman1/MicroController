.include "m2560def.inc"
.list

.CSEG
.org 0x0000
	rjmp start


start:
	ldi r16, low(RAMEND)
	out spl, r16
	ldi r16, high(RAMEND)
	out sph, r16

	ldi r16, 0b11111111
	out DDRA, r16

	ldi r16, 0b11111111
	out DDRF, r16

	ldi r16, 0b00001000 ; buzzer
	sts DDRK, r16

	ldi r16, 0b00101000 ; buttons
	sts PORTL, r16

	.def timerReg = r18

	.def modeA = r19
	.def modeF = r20

	.def ledA = r21
	.def ledF = r22
	ldi ledF, 0b10000000

	.def counterAH = r23
	ldi counterAH, 0b10000000
	.def counterAL = r24
	ldi counterAL, 0b00000001
	.def counterF = r25
	ldi counterF, 0b10000000


loop:
	lds r16, PINL
	sbrs r16, 3
    rcall clickA
	lds r16, PINL
	sbrs r16, 5
	rcall clickF
	sbrc modeA, 0
	rcall ledAswitch
	sbrc modeF, 0
	rcall ledFswitch
	cpi counterAH, 0b00001000
	breq resetA
	contA:
	cpi counterF, 0b00000000
	breq resetF
	contF:
	ldi r16, 0b00000000
	sts PORTK, r16
	rjmp loop


clickA:
	ldi r16, 0b00001000
	sts PORTK, r16
	sbrs modeA, 0
	rjmp setAlgo3
	ldi ledA, 0b00000000
	out PORTA, ledA
	ldi counterAH, 0b10000000
	ldi counterAL, 0b00000001
	ret

setAlgo3:
	ldi modeA, 0b00000001
	ret

clickF:
	ldi r16, 0b00001000
	sts PORTK, r16
	sbrs modeF, 0
	rjmp setAlgo6
	ldi r16, 0b00000000
	out PORTF, r16
	ldi ledF, 0b10000000
	ldi counterF, 0b10000000
	ret

setAlgo6:
	ldi modeF, 0b00000001
	ret

ledAswitch: ; algo3
	ldi ledA, 0b00000000
	or ledA, counterAH
	or ledA, counterAL
	out PORTA, ledA
	lsr counterAH
	lsl counterAL
	rcall timer4
	ret

resetA:
	ldi ledA, 0b00000000
	out PORTA, ledA
	ldi counterAH, 0b10000000
	ldi counterAL, 0b00000001
	ldi modeA, 0b00000000
	rcall timer4
	rjmp contA

ledFswitch: ; algo6
	out PORTF, ledF
	lsr ledF
	lsr ledF
	lsr counterF
	rcall timer4
	cpi counterF, 0b00001000
	breq setFcontinue
	ret

setFcontinue:
	ldi ledF, 0b01000000
	ret

resetF:
	ldi r16, 0b00000000
	out PORTF, r16
	ldi ledF, 0b10000000
	ldi counterF, 0b10000000
	ldi modeF, 0b00000000
	rcall timer4
	rjmp contF

timer4:
	ldi timerReg, 0b00000000
	or timerReg, modeA
	eor timerReg, modeF
	sbrs timerReg, 0
	rjmp lowSpeed
	.equ freq = 58500 ; 0,9 sec
	// (16/8) * 64 * 10^(-6) * (2^16 - 58500)
	ldi timerReg, high(freq)
	sts TCNT4H, timerReg
	ldi timerReg, low(freq)
	sts TCNT4L, timerReg

	contFreq:
	ldi timerReg, 0b00000000
	sts TCCR4A, timerReg
	ldi timerReg, 0b00000101 ; OVF 1024
	sts TCCR4B, timerReg
	rcall ovf4
	ret

ovf4:
	in timerReg, TIFR4
	sbrs timerReg, TOV4
	rjmp delayClicks

	ldi timerReg, (1 << TOV4)
	out TIFR4, timerReg

	ldi timerReg, 0b00000000
	sts TCCR4B, timerReg
	ret

delayClicks:
	lds r16, PINL
	sbrs r16, 3
	rcall clickA
	lds r16, PINL
	sbrs r16, 5
	rcall clickF
	rjmp ovf4

lowSpeed:
	.equ freq2 = 62020 ; 0,45 sec
	// (16/8) * 64 * 10^(-6) * (2^16 - 62020)
	ldi timerReg, high(freq2)
	sts TCNT4H, timerReg
	ldi timerReg, low(freq2)
	sts TCNT4L, timerReg
	
	rjmp contFreq

	