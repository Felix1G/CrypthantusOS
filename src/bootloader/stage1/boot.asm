org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

;
; -------
;	FAT
; -------
;

jmp 	short main
nop

bdb_oem:			db 'MSWIN4.1' 	; max compatibility
bdb_sector_bytes: 	dw 512
bdb_cluster_sectors:db 1
bdb_res_sectors: 	dw 1 			; reserved sectors
bdb_fats: 			db 2 			; fat count
bdb_dir_entries: 	dw 0E0h 		; entry count
bdb_total_sectors: 	dw 2880 		; 2880 * 512B = 1.4MB
bdb_media_desc_t: 	db 0F0h 		; F0 -> 3.5" floppy
bdb_fat_sectors: 	dw 9 			; 9 sectors/fat
bdb_track_sectors:  dw 18
bdb_heads: 			dw 2
bdb_hidden_sectors: dd 0
bdb_large_sectors: 	dd 0

; extended boot record
ebr_drive_num: 		db 0 			; 0 = floppy disk, 0x80 = hdd
ebr_res: 			db 0 			; reserved
ebr_sig:			db 29h			; signature
ebr_vol_id: 		db 12h, 34h, 56h, 78h ; volume id
ebr_vol_label: 		db 'CRYPTHANTUS'; 11 bytes
ebr_sys_id: 		db 'FAT12      '; system id


;
; --------
;	MAIN
; --------
;

main:
	; data segments
	mov 	ax, 0
	mov 	ds, ax
	mov		es, ax

	; stack
	mov 	ss, ax
	mov 	sp, 0x7C00 			; stack goes backwards

	push 	es
	push 	word .after 		; make sure bios is at 0000:7C00h
	retf
	
.after:
	mov 	[ebr_drive_num], dl
	
	mov 	si, msg_boot
	call 	puts

	; read drive parameters
	; cannot rely data on formatted disk
	push 	es
	mov 	ah, 08h 			; get drive parameters
	int 	13h
	jc		err_disk_read
	pop 	es

	and 	cl, 0x3F 			; remove top 2 bits
	xor 	ch, ch
	mov 	[bdb_track_sectors], cx

	inc 	dh
	mov 	[bdb_heads], dh

	; compute FAT root directory
	mov 	ax, [bdb_fat_sectors]
	mov 	bl, [bdb_fats]
	xor 	bh, bh
	mul 	bx 					; bx *= ax -> total sectors
	add 	ax, [bdb_res_sectors]
	push 	ax 					; LBA of root dir

	mov 	ax, [bdb_fat_sectors]
	shl 	ax, 5 				; ax *= 32
	xor 	dx, dx
	div 	word [bdb_sector_bytes] ; number of sectors to read

	test 	dx, dx 				
	jz 		.read_root_dir
	inc 	ax 					; if division remainder != 0, add 1
								; meaning entries partially filled in sectors

.read_root_dir:
	mov 	cl, al				; cl = number of sectors to read
	pop 	ax 					; LBA of root dir
	mov		dl, [ebr_drive_num]
	mov 	bx, buffer
	call 	disk_read

	; search for kernel.bin
	xor 	bx, bx
	mov 	di, buffer	

.search_boot_bin:
	mov		si, filename_boot_bin
	mov 	cx, 11 				; compare up to 11 chars
	push 	di
	repe	cmpsb
	pop 	di
	je 		.found_boot_bin

	add 	di, 32
	inc 	bx
	cmp 	bx, [bdb_dir_entries]
	jl		.search_boot_bin	; there are more to search for

	jmp 	err_boot_missing

.found_boot_bin:
	; di should have the address to the entry
	mov 	ax, [di + 26]		; first logical cluster field
	mov		[boot_cluster], ax

	; load FAT from disk to memory
	mov 	ax, [bdb_res_sectors]
	mov 	bx, buffer
	mov		cl, [bdb_fat_sectors]
	mov 	dl, [ebr_drive_num]
	call 	disk_read

	; read kernel and process FAT chain
	mov 	bx, BOOT_LOAD_SEGMENT
	mov 	es, bx
	mov 	bx, BOOT_LOAD_OFFSET

.load_boot_loop:
	mov 	ax, [boot_cluster] 	; read next cluster
	add 	ax, 31 				; hardcoded value :(

	mov 	cl, 1
	mov 	dl, [ebr_drive_num]
	call 	disk_read

	add 	bx, [bdb_sector_bytes]

	mov		ax, [boot_cluster]
	mov 	cx, 3
	mul 	cx
	mov 	cx, 2
	div 	cx 					; ax = index of entry, dx = cluster mod 2

	mov 	si, buffer
	add 	si, ax
	mov 	ax, [ds:si]			; read entry from FAT table at index ax

	or 		dx, dx
	jz 		.even

.odd:
	shr 	ax, 4
	jmp 	.next_cluster_done

.even:
	and 	ax, 0x0FFF

.next_cluster_done:
	cmp 	ax, 0x0FF8
	jae 	.finish_boot_read 	; end of chain

	mov 	[boot_cluster], ax
	jmp 	.load_boot_loop 	; loop the chain again

.finish_boot_read:
	mov 	dl, [ebr_drive_num]	; boot device in dl

	mov 	ax, BOOT_LOAD_SEGMENT ; set segment registers
	mov 	ds, ax
	mov 	es, ax

	jmp 	BOOT_LOAD_SEGMENT:BOOT_LOAD_OFFSET

	jmp 	key_press_reboot 	; should never happen

	jmp 	halt_prog


;
; ---------
;	ERROR
; ---------
;

err_disk_read:
	mov 	si, msg_err_disk_read
	call	puts
	jmp 	key_press_reboot

err_boot_missing:
	mov 	si, msg_err_boot_missing
	call 	puts
	jmp		key_press_reboot

key_press_reboot:
	mov		ah, 0
	int 	10h
	jmp 	0FFFFh:0

halt_prog:
	cli
	hlt

.halt:
	jmp 	.halt 				; infinite loop if halt fails


;
; ------
;	IO
; ------
;

; -------------
; prints a string.
puts:
	push 	si
	push	ax
	push 	bx

.loop:
	lodsb 						; load next char in al
	or 		al, al 				; verify if next char is null
	jz 		.done

	mov 	ah, 0x0e 			; print code
	mov 	bh, 0 				; page num 0
	int 	10h

	jmp 	.loop

.done:
	pop 	bx
	pop 	ax
	pop 	si
	ret

; -------------
; converts LBA address to CHS address.
; parameters:
; 	ax		-		LBA address
; returns:
;	cx [bit 0-5] 	-		sector num
;	cx [bit 6-15] 	-		cylinder
;	dh 				-		head
lba_to_chs:
	push 	ax
	push 	dx

	xor 	dx, dx 				; dx = 0
	div 	word [bdb_track_sectors] ; LBA /= sector/track

	inc 	dx					; dx++ = sector
	mov 	cx, dx 				

	xor 	dx, dx 				; dx = 0
	div 	word [bdb_heads]	; LBA /= heads
								; LBA %= heads

	mov 	dh, dl 				; dh = dl = head
	mov 	ch, al				; ch = cylinder (lower 8 bits)
	shl		ah, 6
	or 		cl, ah				; upper 2 bits ah

	pop 	ax
	mov 	dl, al
	pop 	ax
	ret

; -------------
; read sectors from the disk.
; parameters:
;	ax    -		LBA address
;	cl 	  - 	number of sectors to read (max 128)
; 	dl    - 	drive number
; 	es:bx - 	memory address to store data
disk_read:
	push	ax
	push 	bx
	push 	cx
	push 	dx
	push 	di

	push	cx
	call 	lba_to_chs
	pop 	ax 					; ax = cx = number of sectors to read

	mov 	ah, 02h				; read disk sector
	mov 	di, 3 				; do 3 retries

.retry:
	pusha

	stc							; make sure BIOS sets the carry flag
	int		13h
	jnc 	.done

	popa 						; read failed
	call 	disk_reset

	dec 	di
	test 	di, di
	jnz 	.retry

.fail:
	jmp 	err_disk_read

.done:
	popa

	pop 	di 					; restore
	pop 	dx
	pop 	cx
	pop 	bx
	pop 	ax
	ret

; -------------
; reset disk controller
; parameters:
; 	dl - drive number
disk_reset:
	pusha
	mov 	ah, 0
	stc
	int 	13h
	jc 		err_disk_read
	popa
	ret

;
; --------
;	DATA
; --------
;

msg_boot: 					db 'Booting up!', ENDL, 0
msg_err_disk_read:			db 'Cannot read disk!', ENDL, 0
msg_err_boot_missing:		db 'Missing next stage!', ENDL, 0
filename_boot_bin: 			db 'STAGE2  BIN'
boot_cluster: 				db 0

BOOT_LOAD_SEGMENT			equ 0x0
BOOT_LOAD_OFFSET 			equ 0x500


;
; ------------------
;	SECTOR PADDING
; ------------------
;
times 510-($-$$) db 0 ; 512 - reserved bytes
dw 0AA55h


buffer:
