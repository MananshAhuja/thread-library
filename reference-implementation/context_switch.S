.text
.globl context_switch
.type context_switch, @function

context_switch:
#save the current context (old_contect is in rdi)

mov %rsp, 0(%rdi)    # current->rsp
mov %rbp, 8(%rdi)    # current->rbp
mov %rbx,16(%rdi)
mov %r12,24(%rdi)
mov %r13,32(%rdi)
mov %r14,40(%rdi)
mov %r15,48(%rdi)


#restore the new context (new_context is in %rsi)

movq 0(%rsi), %rsp
movq 8(%rsi), %rbp
movq 16(%rsi), %rbx
movq 24(%rsi), %r12
movq 32(%rsi), %r13
movq 40(%rsi), %r14
movq 48(%rsi), %r15
movq 56(%rsi), %rdi

#resume the new context
ret
.section .note.GNU-stack,"",@progbits