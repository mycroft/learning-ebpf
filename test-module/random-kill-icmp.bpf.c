#define __x86_64__

#include <stdint.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/bpf.h>
#include <stdlib.h>
#include <bpf/bpf_helpers.h>

#define OVER(x, d) (x + 1 > (typeof(x))d)

static inline void csum_replace2(uint16_t *sum, uint16_t old, uint16_t new)
{
	uint16_t csum = ~*sum;

	csum += ~old;
	csum += csum < (uint16_t)~old;

	csum += new;
	csum += csum < (uint16_t)new;

	*sum = ~csum;
}

SEC("xdp")
int random_kill_icmp(struct xdp_md *ctx) {
	void *data_end = (void *)(uintptr_t)ctx->data_end;
	void *data = (void *)(uintptr_t)ctx->data;
	uint8_t old_ttl;

	struct ethhdr *eth = data;
	struct iphdr *iph = (struct iphdr *)(eth + 1);
	struct icmphdr *icmph = (struct icmphdr *)(iph + 1);

	/* sanity check needed by the eBPF verifier */
	if (OVER(eth, data_end))
		return XDP_DROP;

	if (eth->h_proto != ntohs(ETH_P_IP))
		return XDP_PASS;

	/* sanity check needed by the eBPF verifier */
	if (OVER(iph, data_end))
		return XDP_DROP;

	if (iph->protocol != IPPROTO_ICMP)
		return XDP_PASS;

	/* sanity check needed by the eBPF verifier */
	if (OVER(icmph, data_end))
		return XDP_DROP;

	/* let other ICMP pass */
	if (icmph->type != ICMP_ECHO && icmph->type != ICMP_ECHOREPLY)
		return XDP_PASS;

	switch (icmph->type) {
		case ICMP_ECHO:
			/* randomly kill some requests */
			if(bpf_get_prandom_u32() % 4 == 0) {
				bpf_printk("Dropped icmp echo seq: %d", ntohs(icmph->un.echo.sequence));
				return XDP_DROP;
			}
			break;
		case ICMP_ECHOREPLY:
			/* set the TTL to a pseudorandom number 1..255 */
			old_ttl = iph->ttl;
			iph->ttl = bpf_get_prandom_u32() & 0xff ?: 1;

			/* recalculate the checksum, otherwise the IP stack wil drop it */
			csum_replace2(&iph->check, htons(old_ttl << 8), htons(iph->ttl << 8));
			bpf_printk("Applying new TTL: %d", iph->ttl);

			break;
	}

    return XDP_PASS;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// sudo bpftool prog load random-kill-icmp.bpf.o /sys/fs/bpf/random-kill-icmp
// sudo bpftool net attach xdp name random_kill_icmp dev lo
//
// sudo bpftool net detach xdp dev lo
// sudo rm /sys/fs/bpf/random-kill-icmp
