//go:build ignore

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

struct bpf_map_def {
	unsigned int type;
	unsigned int key_size;
	unsigned int value_size;
	unsigned int max_entries;
	unsigned int map_flags;
};

typedef unsigned int u32;
typedef long long unsigned int u64;

char __license[] SEC("license") = "Dual MIT/GPL";

struct bpf_map_def SEC("maps") kprobe_map = {
	.type        = 2,
	.key_size    = sizeof(u32),
	.value_size  = sizeof(u64),
	.max_entries = 1,
};

SEC("kprobe/sys_execve")
int kprobe_execve() {
	u32 key     = 0;
	u64 initval = 1, *valp;

	valp = bpf_map_lookup_elem(&kprobe_map, &key);
	if (!valp) {
		bpf_map_update_elem(&kprobe_map, &key, &initval, BPF_ANY);
		return 0;
	}
	__sync_fetch_and_add(valp, 1);

	return 0;
}