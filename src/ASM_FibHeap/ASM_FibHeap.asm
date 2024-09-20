extern calloc
extern free

extern stackCtor
extern stackDtor
extern stackPush
extern stackPop
extern stackTop
extern stackSize

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
; SPOIL: according to the agreement
; caller saved registers
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
; SPOIL: according to the agreement
; caller saved registers
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
; SPOIL: according to the agreement
; caller saved registers
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
; SPOIL: according to the agreement
; caller saved registers
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
; FUNC: fibHeap insert
; FibNode* fibHeapIns(FibHeap* heap, Key_t key) 
;
; INPUT: rdi = heap, rsi = key
; OUTPUT: rax = new_node pointer
; SPOIL: according to the agreement
; caller saved registers
;
;-------------------------
ASM_fibHeapIns:
    ; TODO: add a function
    push    rbx ; saving register for recovery


    test    rdi,    rdi     ; if(haep == NULL)
    jz ASM_fibHeapIns_EXIT  ;   return


    mov     rbx,    rdi ; rbx = heap


    mov     rdi,    rsi     ; rdi = key
    call ASM_fibNodeInit    ; rax = new_node


    cmp qword [rbx + fh_size_offset], 0
    jne ASM_fibHeapIns_1_if
        mov [rbx + fh_min_offset], rax
        jmp ASM_fibHeapIns_1_if_end
    ASM_fibHeapIns_1_if:
        mov rsi, [rbx + fh_min_offset]
        mov rdi, rax

    ASM_fibHeapIns_1_if_end:


ASM_fibHeapIns_EXIT:
    pop     rbx ; restore spoiled register
    ret
;-------------------------


;-------------------------
;
; FUNC: fibNode destructor
;
; static inline void fibNodeDtor(FibNode* node)
;
; INPUT: rdi = node
; OUTPUT: NONE
; SPOIL: according to the agreement
; caller saved registers
;
;-------------------------
ASM_fibNodeDtor:
    push    rbx ; saving register for recovery
    push    rbp ;
    push    r12 ;


    test    rdi,    rdi     ; if(node == NULL)
    jz ASM_fibNodeDtor_EXIT ;   return


    mov     rbx,    rdi     ; rbx = node


    call stackCtor          ; rax = stk = stackCtor
    test    rax,    rax     ; if(stk == NULL)
    jz ASM_fibNodeDtor_EXIT ;   return


    mov     rbp,    rax     ; rbp = stk


    mov     r12,    rbx     ; r12 = node = first_node


    ASM_fibNodeDtor_LOOP_START:   ; for(;;) {
        

        cmp qword [rbx + fn_child_offset], 0        ; if(node->child != NULL) {
        je ASM_fibNodeDtor_1_if                     ;
            mov     rdi,    rbp                     ; rdi = stk
            mov     rsi,    [rbx + fn_child_offset] ; rsi = node->child
            call stackPush                          ; stackPush(stk, node->child)
        ASM_fibNodeDtor_1_if:                       ; }


        mov     rdi,    rbp                     ; rdi = stk
        mov     rsi,    [rbx + fn_right_offset] ; rsi = node->right
        call stackPush                          ; stackPush(stk, node->right)


        mov     rdi,    rbx ; rdi = node
        call free WRT ..plt ; free(node)


        mov     rdi,    rbp ; rdi = stk
        call stackTop       ; rax = stackTop(stk)
        mov     rbx,    rax ; rbx = rax = node


        mov     rdi,    rbp ; rdi = stk
        call stackPop       ; stackPop(stk)


        cmp     rbx,    r12         ; if(node == first_node) {
        jne ASM_fibNodeDtor_2_if    ;


            mov     rdi,    rbp         ; rdi = stk
            call stackSize              ; rax = stackSize(stk)
            cmp     rax,    0           ; if(stackSize(stk) == 0)
            je ASM_fibNodeDtor_LOOP_END ;   break


            mov     rdi,    rbp ; rdi = stk
            call stackTop       ; rax = stackTop(stk)
            mov     rbx,    rax ; rbx = rax = node


            mov     rdi,    rbp ; rdi = stk
            call stackPop       ; stackPop(stk)


            mov     r12,    rbx ; first_node = node


        ASM_fibNodeDtor_2_if:
    jmp ASM_fibNodeDtor_LOOP_START ; }
    ASM_fibNodeDtor_LOOP_END:


    mov     rdi,    rbp ; rdi = stk
    call stackDtor      ; stackDtor(stk)


ASM_fibNodeDtor_EXIT:
    pop     r12 ; restore spoiled register
    pop     rbp ; 
    pop     rbx ; 
    ret

section .data