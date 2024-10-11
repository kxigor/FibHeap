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
    jz ASM_fibHeapCtor_exit     ;   return NULL


    mov     rbx,    rax    ; rbx = rax = heap i.e. heap pointer


    mov     rdi,    size_nodes_arr      ; rdi = ASMFIBARRSTARTSIZE = 50000
    mov     rsi,    8                   ; rsi = sizeof(FibNode*) = 8 i.e. pointer
    call calloc WRT ..plt               ; rax = calloc(rdi, rsi) = calloc(50000, sizeof(FibNode*))
    test    rax,    rax                 ; if(rax == NULL)
    jz ASM_fibHeapCtor_1_if_start   ; {free(heap); return NULL}


    jmp ASM_fibHeapCtor_1_if_end    ; No need to free up memory


    ; Accordingly, we allocated memory 
    ; for the first pointer, but we couldn't 
    ; for the second one, so we need to free 
    ; up for the first one
    ASM_fibHeapCtor_1_if_start:
    mov     rdi,    rbx         ; rdi = rax = heap i.e. heap pointer
    call free WRT ..plt         ; free(heap)
    jmp ASM_fibHeapCtor_exit    ; Exiting the function
    ASM_fibHeapCtor_1_if_end:


    mov     [rbx + fh_array_offset],    rax  ; heap->array = calloc(...)
    mov     rax,                        rbx  ; rax = heap i.e. for return


    ASM_fibHeapCtor_exit:


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
    push rdi ; push key


    mov rdi, 1               ; rdi = 1 = number of FibNodes
    mov rsi, sizeof_FibNode  ; rsi = 56 = sizeof(FibNode)
    call calloc WRT ..plt    ; rax = calloc(rdi, rsi) = calloc(1, sizeof(FibNode))


    test rax, rax           ; if(rax == NULL)
    jz ASM_fibNodeInit_exit ;   reutrn NULL;

    pop rdi                             ; rdi = key
    mov [rax + fn_key_offset], rdi      ; node->key = key
    mov [rax + fn_left_offset], rax     ; node->left = node
    mov [rax + fn_right_offset], rax    ; node->right = node
    

    ASM_fibNodeInit_exit:

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
    call ASM_fibNodeInit                ; rax = fibNodeInit(rdi), rdi = key
    push rax                            ; push node
    call ASM_fibHeapCtor                ; rax = fibHeapCtor
    pop rdx                             ; rdx = node
    mov [rax + fh_min_offset], rdx      ; heap->min = fibNodeInit(rdi), rdi = key
    mov qword [rax + fh_size_offset], 1 ; heap->size = 1
    ret                                 ; return heap

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
    push rdi                        ; push heap
    mov rdi, [rdi + fh_min_offset]  ; rdi = heap->min
    call ASM_fibNodeDtor            ; free(rdi) = free(heap->min)


    pop rdi                             ; rdi = heap
    push rdi                            ; push heap
    mov rdi, [rdi + fh_array_offset]    ; rdi = heap->array
    call free WRT ..plt                 ; free(rdi) = free(heap->array)


    pop rdi             ; rdi = heap
    call free WRT ..plt ; free(rdi) = free(heap)


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
    push rbx ; saving register for recovery


    test rdi, rdi     ; if(haep == NULL)
    jz ASM_fibHeapIns_exit  ;   return


    mov rbx, rdi ; rbx = heap


    mov rdi, rsi            ; rdi = key
    call ASM_fibNodeInit    ; rax = new_node


    cmp qword [rbx + fh_size_offset], 0 ; if(heap->size == 0) {
    jne ASM_fibHeapIns_1_if             ;
        mov [rbx + fh_min_offset], rax  ; heap->min = rax = new_node
        jmp ASM_fibHeapIns_1_if_end     ;
    ASM_fibHeapIns_1_if:                ; } else {
        ; push rax PS rax not spoil
        mov rsi, [rbx + fh_min_offset]  ; rsi = heap->min
        mov rdi, rax                    ; rdi = new_node
        call ASM_fibNodeUnionLists      ; fibNodeUnionLists(heap->min, new_node)
        ; pop rax PS rax not spoil


        mov rdx, [rbx + fh_min_offset]      ; rdx = rbx + fh_min_offset = heap->min
        mov rdx, [rdx + fn_key_offset]      ; rdx = rdx + fn_key_offset = heap->min->key
        cmp rdx, [rax + fn_key_offset]      ; if(heap->min->key > new_node->key)
        jle ASM_fibHeap_2_if                ;
            mov [rbx + fh_min_offset], rax  ; heap->min = new_node;
        ASM_fibHeap_2_if:                   ;
    ASM_fibHeapIns_1_if_end:            ; }


    inc qword [rbx + fh_size_offset]


    ASM_fibHeapIns_exit:
    pop rbx ; restore spoiled register
    ret
;-------------------------


;-------------------------
;
; FUNC: fibHeap get min
; FibNode* fibHeapGetMin(const FibHeap* heap)
;
; INPUT: rdi = heap
; OUTPUT: rax = min_node
; SPOIL: according to the agreement
; caller saved registers
;
;-------------------------
ASM_fibHeapGetMin:
    test rdi, rdi                           ; if(heap == NULL)
    jz ASM_fibHeapGetMin_exit               ;   return NULL
    mov rax, [rdi + fh_min_offset]          ; else
    ASM_fibHeapGetMin_exit:                 ;   return heap->min
    ret                                     ;


;-------------------------
;
; FUNC: fibHeap extract min
; void fibHeapExtMin(FibHeap* heap)
;
; INPUT: rdi = heap
; OUTPUT: NONE
; SPOIL: according to the agreement
; caller saved registers
;
;-------------------------
ASM_fibHeapExtMin:
    push rbx ; saving register for recovery


    test rdi, rdi               ; if(heap == NULL)
    jz ASM_fibHeapExtMin_exit   ;   return


    mov rbx, rdi ; rbx = heap


    cmp qword [rbx + fh_size_offset], 0 ; if(heap->size == 0)
    je ASM_fibHeapExtMin_exit           ;   return


    cmp qword [rbx + fh_size_offset], 1         ; if(heap->size == 1) {
    jne ASM_fibHeapExtMin_1_if                  ;
        mov rdi, [rbx + fh_min_offset]          ; rdi = heap->min
        call free WRT ..plt                     ; free(rdi) = free(heap->min)
        mov qword [rbx + fh_min_offset], 0      ; heap->min = 0
        mov qword [rbx + fh_size_offset], 0     ; heap->size = 0
        jmp ASM_fibHeapExtMin_exit              ; return
    ASM_fibHeapExtMin_1_if:                     ; }


    push qword [rbx + fh_min_offset] ; push heap->min i.e. node for delete


    mov rdi, [rbx + fh_min_offset]              ; rdi = heap->min
    cmp qword [rdi + fn_child_offset], 0        ; if(rdi->child != NULL) { //heap->min->child
    je ASM_fibHeapExtMin_2_if                   ;
        mov rsi, [rdi + fn_child_offset]        ; rsi = rdi->child = heap->min->child
        call ASM_fibNodeUnionLists              ;
    ASM_fibHeapExtMin_2_if:                     ; }


    mov rdi, [rbx + fh_min_offset]  ; rdi = heap->min
    call ASM_fibNodeUntie           ; fibNodeUntie(heap->min)


    mov rax, [rbx + fh_min_offset]   ; rax = heap->min
    mov rax, [rax + fn_right_offset] ; rax = heap->min->right
    mov [rbx + fh_min_offset], rax   ; heap->min = heap->min->right


    pop rdi             ; pop node for delete
    call free WRT ..plt ; free(delete_node)


    dec qword [rbx + fh_size_offset] ; heap->size--;


    mov rdi, rbx                ; rdi = heap
    call ASM_fibHeapConsolidate ; fibHeapConsolidate(heap)


    ASM_fibHeapExtMin_exit:
    pop rbx ; restore spoiled register
    ret     ; return

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
    push rbx ; saving register for recovery
    push rbp ;
    push r12 ;


    test rdi, rdi           ; if(node == NULL)
    jz ASM_fibNodeDtor_exit ;   return


    mov rbx, rdi ; rbx = node


    call stackCtor          ; rax = stk = stackCtor
    test rax, rax           ; if(stk == NULL)
    jz ASM_fibNodeDtor_exit ;   return


    mov rbp, rax ; rbp = stk


    mov r12, rbx ; r12 = node = first_node


    ASM_fibNodeDtor_loop_start:   ; for(;;) {
        

        cmp qword [rbx + fn_child_offset], 0    ; if(node->child != NULL) {
        je ASM_fibNodeDtor_1_if                 ;
            mov rdi, rbp                        ; rdi = stk
            mov rsi, [rbx + fn_child_offset]    ; rsi = node->child
            call stackPush                      ; stackPush(stk, node->child)
        ASM_fibNodeDtor_1_if:                   ; }


        mov rdi, rbp                     ; rdi = stk
        mov rsi, [rbx + fn_right_offset] ; rsi = node->right
        call stackPush                   ; stackPush(stk, node->right)


        mov rdi, rbx        ; rdi = node
        call free WRT ..plt ; free(node)


        mov rdi, rbp    ; rdi = stk
        call stackTop   ; rax = stackTop(stk)
        mov rbx, rax    ; rbx = rax = node


        mov rdi, rbp    ; rdi = stk
        call stackPop   ; stackPop(stk)


        cmp rbx, r12                ; if(node == first_node) {
        jne ASM_fibNodeDtor_2_if    ;


            mov rdi, rbp                ; rdi = stk
            call stackSize              ; rax = stackSize(stk)
            cmp rax, 0                  ; if(stackSize(stk) == 0)
            je ASM_fibNodeDtor_loop_end ;   break


            mov rdi, rbp    ; rdi = stk
            call stackTop   ; rax = stackTop(stk)
            mov rbx, rax    ; rbx = rax = node


            mov rdi, rbp    ; rdi = stk
            call stackPop   ; stackPop(stk)


            mov r12, rbx ; first_node = node


        ASM_fibNodeDtor_2_if:
    jmp ASM_fibNodeDtor_loop_start ; }
    ASM_fibNodeDtor_loop_end:


    mov rdi, rbp    ; rdi = stk
    call stackDtor  ; stackDtor(stk)


    ASM_fibNodeDtor_exit:
    pop r12 ; restore spoiled register
    pop rbp ; 
    pop rbx ; 
    ret

;-------------------------
;
; FUNC: Merge nodes lists
; static inline void fibNodeUnionLists(FibNode* first, FibNode* second)
;
; INPUT: rdi = first, rsi = second
; OUTPUT: NONE
; SPOIL: rdx, rcx
;
;-------------------------
ASM_fibNodeUnionLists:


    test rdi, rdi                   ; if(first == NULL)
    jz ASM_fibNodeUnionLists_exit   ;   return
    test rsi, rsi                   ; if(second == NULL)
    jz ASM_fibNodeUnionLists_exit   ;   return 


    mov rdx, [rdi + fn_left_offset]  ; rdx = first->left
    mov rcx, [rsi + fn_right_offset] ; rcx = second->right


    mov [rdx + fn_right_offset], rcx ; first->left->right = second->right
    mov [rcx + fn_left_offset], rdx ; second->right->left = first->left
    mov [rdi + fn_left_offset], rsi ; first->left = second
    mov [rsi + fn_right_offset], rdi ; second->right = first


    ASM_fibNodeUnionLists_exit:
    ret


;-------------------------
;
; FUNC: fibNodeUntie
; static inline void fibNodeUntie(FibNode* node)
;
; INPUT: rdi = node
; OUTPUT: NONE
; SPOIL: rdx, rcx
;
;-------------------------
ASM_fibNodeUntie:
    mov rdx, [rdi + fn_left_offset]     ; rdx = node->left
    mov rcx, [rdi + fn_right_offset]    ; rcx = node->right
    mov [rdx + fn_right_offset], rcx    ; node->left->right = node->right
    mov [rcx + fn_left_offset], rdx     ; node->right->left = node->left
    ret

;-------------------------
;
; FUNC: fibHeapConsolidate
; static inline void fibHeapConsolidate(FibHeap* heap)
; 
; INPUT: rdi = heap
; OUTPUT: NONE
; SPOIL: according to the agreement
; caller saved registers
;
;-------------------------
ASM_fibHeapConsolidate:
    ; TODO: restore registers
    push rbx
    push rbp
    push r12
    push r13


    test rdi, rdi                   ; if(rdi == NULL)
    jz ASM_fibHeapConsolidate_exit  ;   return


    mov r12, rdi                        ; r12 = heap
    xor r13, r13                        ; r13 = next_node
    mov rbx, [rdi + fh_min_offset]      ; rbx = current_node = heap->min
    mov rbp, [rdi + fh_array_offset]    ; rbp = heap->array


    ASM_fibHeapConsolidate_1_loop_start:
    mov rax, [rbx + fn_degree_offset] ; rax = current_node->degree
    shl rax, 3                        ; rax *= 8(sizeof void*) i.e. pointer offset
    cmp rbx, [rbp + rax]              ; if(current_node != heap->arr[current_node->degree]) {
    je ASM_fibHeapConsolidate_1_loop_end;


        mov qword [rbx + fn_parent_offset], 0


        mov rcx, [r12 + fh_min_offset]      ; rcx = heap->min
        mov rcx, [rcx + fn_key_offset]      ; rcx = heap->min->key
        cmp rcx, [rbx + fn_key_offset]      ; if (heap->min->key > current_node->key)
        jle ASM_fibHeapConsolidate_1_if     ;
            mov [r12 + fh_min_offset], rbx  ; heap->min = current_node
        ASM_fibHeapConsolidate_1_if:        ;


        mov r13, [rbx + fn_right_offset]    ; next_node = current_node->right


        ASM_fibHeapConsolidate_2_loop_start:    ; while(heap->array[current_node->degree] != NULL)
        mov rax, [rbx + fn_degree_offset]       ; rax = current_node->degree
        shl rax, 3                              ; rax *= 8(sizeof void*) i.e. pointer offset
        cmp qword [rbp + rax], 0                ; if(heap->array[current_node->degree] != NULL) {
        je ASM_fibHeapConsolidate_2_loop_end    ;

        
            mov rcx, [rbx + fn_key_offset]  ; rcx = current_node->key
            mov rdx, [rbp + rax]            ; rdx = heap->array[current_node->degree]
            cmp rcx, [rdx + fn_key_offset]  ; if(current_node->ket > heap->array[current_node->degree])
            jle ASM_fibHeapConsolidate_2_if ;
                mov rcx, rbx                ; rcx = current
                mov rbx, rdx                ; rbx = heap->array[current_node->degree]
                mov [rbp + rax], rcx        ; heap->arrat[current_node->degree] = current
            ASM_fibHeapConsolidate_2_if:    ;


            cmp [rbp + rax], r13                       ; if(heap->arrat[current_node->degree] == next_node)
            cmove r13, [r13 + fn_right_offset] ; next_node = next_node->right


            mov rdi, rbx            ; rdi = current_node
            mov rsi, [rbp + rax]    ; rsi = heap->array[current_node->degree]
            call ASM_fibNodeLink; fibNodeLink(current_node, heap->arrat[current_node->degree])


            ; - 1 because inside the link the degree increased by 1 and we did not know about it, i.e. the rax register was not spoiled
            mov qword [rbp + rax], 0 ; heap->arrat[current_node->degree - 1] = NULL


        jmp ASM_fibHeapConsolidate_2_loop_start
        ASM_fibHeapConsolidate_2_loop_end:


        mov [rbp + rax], rbx ; heap->arrat[current_node->degree] = current_node
        mov rbx, r13         ; current_node = next_node


    jmp ASM_fibHeapConsolidate_1_loop_start ;
    ASM_fibHeapConsolidate_1_loop_end:      ; }


    mov rbx, [r12 + fh_min_offset]          ; current_node = heap->min
    ASM_fibHeapConsolidate_3_loop_start:    ; do {
        mov rax, [rbx + fn_degree_offset]   ; rax = current_node->degree
        shl rax, 3                          ; rax *= 8(sizeof void*) i.e. pointer offset
        mov qword [rbp + rax], 0            ; heap->array[current_node->degree] = NULL
        mov rbx, [rbx + fn_right_offset]    ; current_node = current_node->right
    cmp rbx, [r12 + fh_min_offset]          ; while(current_node != heap->min)
    jne ASM_fibHeapConsolidate_3_loop_start ; 


    ASM_fibHeapConsolidate_exit:
    pop r13
    pop r12
    pop rbp
    pop rbx
    

    ret

;-------------------------
;
; FUNC: fibNodeLink
; static inline void fibNodeLink(FibNode* first, FibNode* second)
;
; INPUT: rdi = first, rsi = second
; OUTPUT: NONE
; SPOIL; rcx, rdx
;
;-------------------------
ASM_fibNodeLink:
    push rdi                ; push first
    mov rdi, rsi            ; rdi = second
    call ASM_fibNodeUntie   ; fibNodeUntie(second)
    call ASM_fibNodeFixate  ; fibNodeFixate(second)
    pop rdi                 ; rdi = first


    cmp qword [rdi + fn_child_offset], 0    ; if(first->child == NULL) {
    jne ASM_fibNodeLink_1_if                ;
        mov [rdi + fn_child_offset], rsi    ; first->child = second
        jmp ASM_fibNodeLink_1_if_end        ;
    ASM_fibNodeLink_1_if:                   ; } else { 
        push rdi                            ; push first
        mov rdi, [rdi + fn_child_offset]    ; rdi = first->child
        call ASM_fibNodeUnionLists          ; fibNodeUnionLists(first->child, second)
        pop rdi                             ; rdi = first
    ASM_fibNodeLink_1_if_end:               ; }


    inc qword [rdi + fn_degree_offset]      ; first->degree++;
    mov [rsi + fn_parent_offset], rdi       ; second->parent = first


    ret ; return


;-------------------------
;
; FUNC: fibNodeFixate
; static inline void fibNodeFixate(FibNode* node)
;
; INPUT: rdi = node
; OUTPUT: NONE
; SPOIL: NONE
;
;-------------------------
ASM_fibNodeFixate:
    mov [rdi + fn_left_offset], rdi     ; node->left = node
    mov [rdi + fn_right_offset], rdi    ; node->right = node
    ret                                 ; return

ASM_fibHeapDel:
    ; TODO write
    ret

section .data