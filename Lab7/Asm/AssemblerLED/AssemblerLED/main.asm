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

ldi r17, 0b00000000
ldi r18, 0b00000000

ldi r20, 0b11111111

start:
    rjmp ledAswitch
	rjmp start


ledAswitch: rcall timer1
	eor r17, r20
	out PORTA, r17
	rjmp ledAswitch

timer1:
	.equ freq = 58500 ; 0,9 sec
	// (16/8) * 64 * 10^(-6) * (2^16 - 58500)
	ldi r19, (freq >> 8)
	sts TCNT1H, r19
	ldi r19, (freq & 0x00FF)
	sts TCNT1L, r19

	ldi r19, 0b00000000
	sts TCCR1A, r19
	ldi r19, 0b00000101 ; OVF 1024
	sts TCCR1B, r19

ovf1: in r19, TIFR1
	sbrs r19, TOV1
	rjmp ovf1

	ldi r19, (1 << TOV1)
	out TIFR1, r19

	ldi r19, 0b00000000
	sts TCCR1B, r19
	ret

