#!/usr/bin/env python

from bcc import BPF

# This following program will run inside the kernel
program = r"""
int hello(void *ctx) {
    int pid = bpf_get_current_pid_tgid() >> 32;

    bpf_trace_printk("Hello World! New process with PID: %d", pid);
    return 0;
}
"""

# Creating a eBPF program attaching the syscall execve to it.
b = BPF(text=program)

syscall = b.get_syscall_fnname("execve")
print(syscall)
b.attach_kprobe(event=syscall, fn_name="hello")
b.trace_print()
