extern calloc
extern free

extern stackCtor
extern stackPush
extern stackTop
extern stackPop
extern stackDtor

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
; see the h file, there will be offsets
;
; offset to FibNode* min
fh_min_offset: equ 0
; offset to uint64_t size
fh_size_offset: equ 8
; offset to FibNode** array
fh_array_offset: equ 16
;
;-------------------------

;-------------------------
; STRUCT: FibNode
;
; For a detailed description of this structure, 
; see the h file, there will be offsets
;
; offset to Key_t key
fn_key_offset:      equ 0
; offset to FibNode* parent
fn_parent_offset:   equ 8
; offset to FibNode* left
fn_left_offset:     equ 16
; offset to FibNode* right
fn_right_offset:    equ 24
; offset to FibNode child
fn_child_offset:    equ 32
; offset to uint64_t degree
fn_degree_offset:   equ 40
; offset to uint8_t
fn_mark_offset:     equ 48
;
; 7 more bytes of alignment
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
    push    rbx ; Storing registers recovery

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


    mov     [rbx + fh_array_offset],    rax  ; heap->array = calloc(...)
    mov     rax,                        rbx  ; rax = heap i.e. for return


ASM_fibHeapCtor_EXIT:


    pop     rbx ; Restoring registers


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
ASM_fibNodeInit:
    push    rdi ; push key


    mov     rdi,    1               ; rdi = 1 = number of FibNodes
    mov     rsi,    sizeof_FibNode  ; rsi = 56 = sizeof(FibNode)
    call calloc WRT ..plt           ; rax = calloc(rdi, rsi) = calloc(1, sizeof(FibNode))


    test    rax,    rax     ; if(rax == NULL)
    jz ASM_fibNodeInit_EXIT ;   reutrn NULL;

    pop     rdi                             ; rdi = key
    mov     [rax + fn_key_offset],      rdi ; node->key = key
    mov     [rax + fn_left_offset],     rax ; node->left = node
    mov     [rax + fn_right_offset],    rax ; node->right = node
    

ASM_fibNodeInit_EXIT:

    ret ; return rax(node)

;-------------------------
;
; FUNC: init fib heap
; FibHeap* fibHeapInit(Key_t key)
;
; INPUT: RDI = Key_t key
; OUTPUT: rax = fibHeap poiner
; SPOIL: NONE
;
;-------------------------
ASM_fibHeapInit:
    call ASM_fibNodeInit                    ; rax = fibNodeInit(rdi), rdi = key
    push        rax                         ; push node
    call ASM_fibHeapCtor                    ; rax = fibHeapCtor
    pop         rdx                         ; rdx = node
    mov         [rax + fh_min_offset],  rdx ; heap->min = fibNodeInit(rdi), rdi = key
    mov qword   [rax + fh_size_offset], 1   ; heap->size = 1
    ret                                     ; return heap

;-------------------------
;
; FUNC: fibHeap Destructor
;
; void fibHeapDtor(FibHeap* heap)
;
; INPUT: rdi = heap
; OUTPUT: NONE
; SPOIL: NONE
;
;-------------------------
ASM_fibHeapDtor:
    push    rdi                             ; push heap
    mov     rdi,    [rdi + fh_min_offset]   ; rdi = heap->min
    call ASM_fibNodeDtor                    ; free(rdi) = free(heap->min)


    pop     rdi                             ; rdi = heap
    push    rdi                             ; push heap
    mov     rdi,    [rdi + fh_array_offset] ; rdi = heap->array
    call free WRT ..plt                     ; free(rdi) = free(heap->array)


    pop     rdi                             ; rdi = heap
    call free WRT ..plt                     ; free(rdi) = free(heap)


    ret

;-------------------------
;
; FUNC: fibNode destructor
;
; static inline void fibNodeDtor(FibNode* node)
;
; INPUT: rdi = node
; OUTPUT: NONE
; SPOIL: NONE
;
;-------------------------
ASM_fibNodeDtor:
    ; TODO: The function is not completed
    test    rdi,    rdi     ; if(node == NULL)
    jz ASM_fibNodeDtor_EXIT ;   return


    call stackCtor          ; rax = stk = stackCtor
    test    rax,    rax     ; if(stk == NULL)
    jz ASM_fibNodeDtor_EXIT ;   return


ASM_fibNodeDtor_EXIT:
    ret

section .data