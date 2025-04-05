.global copy_and_swap

.type copy_and_swap, @function

copy_and_swap:
    lr.w t0, (a0)
    bne t0, a1, fail
    sc.w t0, a2, (a0)
    bnez t0, copy_and_swap
    li a0, 0
    jr ra

    fail:
    li a0, 1
    jr ra