; ** SID-Wave-Capture **
; by Thorsten Kattanek
; Berlin, 29.12.2012

; Speichert OSC3 Ausgang auf REU für SID Emulation
; Es werden alle Waveformen gecaptured von 0-15 
; $0000-0FFF WaveNr. $00
; $1000-1FFF WaveNr. $01
; $2000-2FFF WaveNr. $02
; $3000-3FFF WaveNr. $03 usw...
; Bei den Daten handelt es sich um die oberen 8Bit der Waveausgabe !
; Als Frequenzwert für das Register wird $1000 gesetzt

!to "sid-wave-capture.prg",cbm

sid = $d400
vic = $d000
reu = $df00

*=$0801

!byte $0c,$08,$dc,$07,$9e
!text "2064"
*=$0810

jmp $2000

        *= $2000    	;Assemble to $2000

	lda #$00	;Bildschirm schwarz
	sta $d020
	sta $d021

	jsr $e544	;Bildschirm löschen

	lda #<Ausgabe3  ;Startmeldung ausgeben
	sta $7c	 	
	lda #>Ausgabe3
	sta $7d
	jsr txtout

	;Testen ob eine REU vorhanden ist !!
	lda #$00        ;Null
	sta $df00       ;nach Register 0
	cmp $df00       ;noch drin?
	beq noreu       ;dann keine REU!

	lda reu
        and #16         ;check bit 4 for REU mem
        cmp #16         ;16 = 256Kbx1 
        beq regcheck    ;yes, touch registers
        bne l1          ;no, 1700 ?
l1      
	lda reu
        and #16         ;check bit 4 for REU mem
        cmp #0
        beq capture     ;reu 1700 found
        bne noreu       ;no ram-type, no reu, no fun ...
regcheck
	lda reu
        ldx #2
loop1   
	txa
	sta $df00,x      ; write to registers 2-5                                         
        inx
        cpx #5
        bne loop1
        ldx #02 
loop2   
	txa 
        cmp $df00,x             
        bne noreu 
        inx
        cpx #5
        bne loop2
	jmp capture   
noreu
	lda #<Ausgabe1 ;REU NOT FOUND ausgeben
	sta $7c	 	
	lda #>Ausgabe1
	sta $7d
	jsr txtout
	rts

	;Mit Aufzeichnung beginnen
capture
	lda #<Ausgabe4  ;Startmeldung ausgeben
	sta $7c	 	
	lda #>Ausgabe4
	sta $7d
	jsr txtout
eingabe
	jsr $ffe4
	beq eingabe
	
	cmp #74
	beq ok
	rts
ok
	sei		;Interrupt verhindern
	lda vic+$11	;VIC Controlreg sichern
	sta d011

	lda #$00	;VIC abschalten
	sta vic+$11

	lda #$00	;Frequenz auf $1000 setzen
	ldx #$10
	sta sid+14	
	stx sid+15

	ldy #$00
loooop
	; REU Installieren
	lda #$1b	;SID Register OSC3 (Adresse im C64)
	ldx #$d4	
	sta reu+2
	stx reu+3	

	lda #$00	;Startadresse in der REU
REU_ADD	ldx #$00	
	sta reu+4
	stx reu+5

	lda #$00
	sta reu+6	;Bank 0 auswählen

	lda #$00	;$1000 Byte übertragen
	ldx #$10
	sta reu+7
	stx reu+8

	lda #$00	;Interrupts nicht erlauben
	sta reu+9

	lda #$80	;Nur REU Adresse eröhen
	sta reu+10

	lda #$08	;Reset Waveform
	sta sid+18	

WAV_ADD	lda #$00	;Wird immer erhöht
	sta sid+18

	;Warten auf Anfang nächte Periode 
	ldx #$f0
wait
	nop 
	nop
	nop
	nop
	nop
	nop
	dex
	bne wait
	
	beq wait1	;Nur wegen 3 Zyklen
wait1	nop
	nop		
	nop

	ldx #%10010000	;Übertragung von C64 nach Reu

	stx reu+1	;Übertragung starten
	
	lda REU_ADD+1
	clc	
	adc #$10
	sta REU_ADD+1
	sta WAV_ADD+1

	iny
	cpy #$10
	bne loooop	

	lda d011	;VIC anschalten
	sta vic+$11

	lda #<Ausgabe2 ;REU NOT FOUND ausgeben
	sta $7c	 	
	lda #>Ausgabe2
	sta $7d
	jsr txtout

	cli		;Interrupt wieder freigeben
	rts
	
;/// Textausgabe: Text >> $7c/7d mit NUll am Ende !
txtout
	pha
	tya
	pha 
	ldy #00
txtout_1
	lda ($7c),y
	beq txtout_end
	jsr $ffd2

	inc $7c
	bne txtout_2
	inc $7d
txtout_2
	jmp txtout_1
txtout_end	
	pla
	tay
	pla
	rts

d011
!byte 0

Ausgabe1
!text 	151,"REU NOT FOUND !",13,13,0
Ausgabe2
!text	151,13,"SAMPLE VORGANG IST BEENDET.",13,0
Ausgabe3
!text 	152,">SID WAVE CAPTURE VON THORSTEN KATTANEK<",13,13,0
Ausgabe4
!text 	151,"ACHTUNG! ES WERDEN ALLE DATEN AUF DER",13,"REU IN BANK 0 GELOESCHT !",13,13,"MOECHTEN SIE FORTFAHREN J/N ?",13,0
