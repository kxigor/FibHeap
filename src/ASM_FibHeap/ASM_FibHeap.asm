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

;-------------------------
; STRUCT: FibHeap
;
; For a detailed description of this structure, 
; see the hd file, there will be offsets
;
; offset to FibNode* min
fh_min_offset: equ 0
; offset to uint64_t size
fh_size_offset: equ 8
; offset to FibNode** array
fh_array_offset: equ 8
;
;-------------------------

;-------------------------
;
; Constants
;
size_nodes_arr: equ 50000
sizeof_FibHeap: equ 24
sizeof_FibNode: equ 56
;-------------------------

section .text

;-------------------------
;
; FUNC: ASM fibHeapCtor
; FibHeap* ASM_fibHeapCtor(void)
;
; INPUT : NONE (void)
; OUTPUT: rax = fibHeap pointer
; SPOIL: NONE
;
;-------------------------
ASM_fibHeapCtor:
    push    rbx ;
    push    rcx ; Storing
    push    rdx ; registers
    push    rsi ; for 
    push    r9  ; recovery
    push    r10 ;
    push    r11 ;


    mov     rdi,    1               ; rdi = 1 = number of fibHeaps
    mov     rsi,    sizeof_FibHeap  ; rsi = 24 = sizeof(FibHeap)
    call calloc WRT ..plt           ; rax = calloc(rdi, rsi) = calloc(1, sizeof(FibHeap))


    test    rax,    rax         ; if(rax == NULL)
    jz ASM_fibHeapCtor_EXIT     ;   return NULL


    mov     rbx,    rax    ; rbx = rax = heap i.e. heap pointer


    mov     rdi,    size_nodes_arr      ; rdi = ASMFIBARRSTARTSIZE = 50000
    mov     rsi,    8                   ; rsi = sizeof(FibNode*) = 8 i.e. pointer
    call calloc WRT ..plt               ; rax = calloc(rdi, rsi) = calloc(50000, sizeof(FibNode*))
    test    rax,    rax                 ; if(rax == NULL)
    jz ASM_fibHeapCtor_freeheap_start   ; {free(heap); return NULL}


    jmp ASM_fibHeapCtor_freeheap_end    ; No need to free up memory


    ; Accordingly, we allocated memory 
    ; for the first pointer, but we couldn't 
    ; for the second one, so we need to free 
    ; up for the first one
ASM_fibHeapCtor_freeheap_start:
    mov     rdi,    rbx         ; rdi = rax = heap i.e. heap pointer
    call free WRT ..plt         ; free(heap)
    jmp ASM_fibHeapCtor_EXIT    ; Exiting the function
ASM_fibHeapCtor_freeheap_end:


    mov     [rbx + fh_min_offset],  rax  ; heap->array = calloc(...)
    mov     rax,                    rbx  ; rax = heap i.e. for return


ASM_fibHeapCtor_EXIT:


    pop     r11     ;
    pop     r10     ;
    pop     r9      ; Restoring
    pop     rsi     ; registers
    pop     rdx     ;
    pop     rcx     ;
    pop     rbx     ;


    ret     ; return rax(heap)

;-------------------------
;
; FUNC: init fib node
; static inline FibNode* fibNodeInit(Key_t key)
;
; INPUT: RDI = Key_t key
; OUTPUT: rax = fibNode poiner
; SPOIL: NONE
;
;-------------------------
ASM_FibNodeInit:
    
    ret

section .data