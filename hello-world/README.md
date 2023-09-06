# hello-world: hello.py

To run `hello.py`, it is required to install `bcc` at first:

## Initialization

```shell
$ sudo dnf install -y bcc
```

Then, run the testing program using root priv:

```shell
$ sudo python ./hello.py
```

Everytime a process is launched, the following will show:

```shell
~/d/l/hello-world > sudo python hello.py
b'           <...>-13626   [002] ...21  4956.113633: bpf_trace_printk: Hello World!'
b'           <...>-13626   [002] ...21  4956.113810: bpf_trace_printk: Hello World!'
b'           <...>-13626   [002] ...21  4956.113876: bpf_trace_printk: Hello World!'
b'           <...>-13626   [002] ...21  4956.113939: bpf_trace_printk: Hello World!'
b'           <...>-13626   [002] ...21  4956.114002: bpf_trace_printk: Hello World!'
b'           <...>-13626   [002] ...21  4956.114062: bpf_trace_printk: Hello World!'
b'           <...>-13626   [002] ...21  4956.114123: bpf_trace_printk: Hello World!'
b'           <...>-13626   [002] ...21  4956.114183: bpf_trace_printk: Hello World!'
b'           <...>-13629   [002] ...21  4964.300647: bpf_trace_printk: Hello World!'
b'           <...>-13629   [002] ...21  4964.301934: bpf_trace_printk: Hello World!'
```

This is the result of calling `trace_print`. You can see this content as well by reading the `/sys/kernel/debug/tracing/trace_pipe` file:

```shell
~/d/learning-ebpf (main* ◉) > sudo cat /sys/kernel/debug/tracing/trace_pipe                         12:40:25
           <...>-14457   [004] ...21  5333.536855: bpf_trace_printk: Hello World!
           <...>-14458   [004] ...21  5333.542981: bpf_trace_printk: Hello World!
           <...>-14459   [006] ...21  5333.544131: bpf_trace_printk: Hello World!
           <...>-14460   [008] ...21  5333.547008: bpf_trace_printk: Hello World!
           <...>-14461   [004] ...21  5333.548709: bpf_trace_printk: Hello World!
```

## Samples

* `hello.py`: Sample prink, PID retrieval;
* `hello-hashmap.py`: Sends data from kernel space to user space in a map; Hashmap usage;
* `hello-perf-output.py`: Sends data from kernel space to user space in a ring buffer; More process related information retrieval