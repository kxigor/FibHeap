extern calloc
extern free

global ASM_fibHeapCtor
global ASM_fibHeapInit
global ASM_fibHeapDtor
global ASM_fibHeapIns
global ASM_fibHeapDel
global ASM_fibHeapGetMin
global ASM_fibHeapExtMin
global ASM_fibHeapMerge
global ASM_fibHeapGetSize
global ASM_fibHeapOverrideKey

ASMFIBARRSTARTSIZE: equ 50000

;-------------------------
; STRUCT: FibHeap
;
; For a detailed description of this structure, 
; see the hd file, there will be offsets
;
; offset to FibNode* min
FibHeap_min_offset: equ 0
; offset to uint64_t size
FibHeap_size_offset: equ 8
; offset to FibNode** array
FibHeap_array_offset: equ 8
;
;-------------------------

section .text

;-------------------------
;
; FUNC: ASM fibHeapCtor
; FibHeap* ASM_fibHeapCtor(void)
;
; INPUT : NONE (void)
; OUTPUT: rax (fibHeap pointer)
; SPOIL: NONE
;
;-------------------------
ASM_fibHeapCtor:
    ; TODO Save Registers
    ; rbx, rcx, rdx, rsi, rdi, r9, r10, r11

    mov rdi, 1              ; rdi = 1 = number of fibHeaps
    mov rsi, 24             ; rsi = 24 = sizeof(FibHeap)
    call calloc WRT ..plt   ; rax = calloc(rdi, rsi) = calloc(1, sizeof(FibHeap))

    test rax, rax           ; if(rax == NULL)
    jz ASM_fibHeapCtor_EXIT ;   return NULL

    mov rbx, rax    ; rbx = rax = heap i.e. heap pointer

    mov rdi, ASMFIBARRSTARTSIZE ; rdi = ASMFIBARRSTARTSIZE = 50000
    mov rsi, 8                  ; rsi = sizeof(FibNode*) = 8 i.e. pointer
    call calloc WRT ..plt       ; rax = calloc(rdi, rsi) = calloc(50000, sizeof(FibNode*))
    test rax, rax                       ; if(rax == NULL)
    jz ASM_fibHeapCtor_freeheap_start   ; {free(heap); return NULL}

    jmp ASM_fibHeapCtor_freeheap_end

    ; Accordingly, we allocated memory 
    ; for the first pointer, but we couldn't 
    ; for the second one, so we need to free 
    ; up for the first one
ASM_fibHeapCtor_freeheap_start:
    mov rdi, rbx        ; rdi = rax = heap i.e. heap pointer
    call free WRT ..plt ; free(heap)
    jmp ASM_fibHeapCtor_EXIT
ASM_fibHeapCtor_freeheap_end:

    mov [rbx + FibHeap_min_offset], rax
    mov rax, rbx
    ; TODO Restore Registers

ASM_fibHeapCtor_EXIT:
    ret

section .data