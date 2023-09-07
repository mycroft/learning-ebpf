# test-c

## Compilation

```sh
$ makefile
...
```

## Loading & checking

```sh
$ sudo bpftool prog load hello.bpf.o /sys/fs/bpf/hello
$ sudo bpftool prog list
...
191: xdp  name hello  tag d35b94b4c0c10efb  gpl
	loaded_at 2023-09-06T19:28:33+0200  uid 0
	xlated 96B  jited 72B  memlock 4096B  map_ids 76,77
	btf_id 330

$ sudo bpftool prog show id 191 --pretty
{
    "id": 191,
    "type": "xdp",
    "name": "hello",
    "tag": "d35b94b4c0c10efb",
    "gpl_compatible": true,
    "loaded_at": 1694021313,
    "uid": 0,
    "bytes_xlated": 96,
    "jited": true,
    "bytes_jited": 72,
    "bytes_memlock": 4096,
    "map_ids": [76,77
    ],
    "btf_id": 330
}

$ sudo bpftool prog dump xlated name hello
int hello(struct xdp_md * ctx):
; bpf_printk("Hello World %d", counter);
   0: (18) r6 = map[id:76][0]+0
   2: (61) r3 = *(u32 *)(r6 +0)
   3: (18) r1 = map[id:77][0]+0
   5: (b7) r2 = 15
   6: (85) call bpf_trace_printk#-72416
; counter++;
   7: (61) r1 = *(u32 *)(r6 +0)
   8: (07) r1 += 1
   9: (63) *(u32 *)(r6 +0) = r1
; return XDP_PASS;
  10: (b7) r0 = 2
  11: (95) exit

```

## Attaching

```sh
$ sudo bpftool net attach xdp id 191 dev wlp0s20f3
s sudo bpftool net list
xdp:
wlp0s20f3(3) generic id 191

$ ip link show wlp0s20f3
3: wlp0s20f3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 xdpgeneric qdisc noqueue state UP mode DORMANT group default qlen 1000
    link/ether bc:6e:e2:05:19:28 brd ff:ff:ff:ff:ff:ff
    prog/xdp id 191

```

## Viewing counter & other stuff

```sh
$ sudo bpftool map dump name hello.bss
[{
        "value": {
            ".bss": [{
                    "counter": 447
                }
            ]
        }
    }
]

$ sudo bpftool map dump name hello.rodata
[{
        "value": {
            ".rodata": [{
                    "hello.____fmt": "Hello World %d"
                }
            ]
        }
    }
]
```


## Removing

```sh
$ sudo bpftool net detach xdp dev wlp0s20f3
$ sudo rm /sys/fs/bpf/hello
$ sudo bpftool prog show name hello
$
```


