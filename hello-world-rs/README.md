# ebpf with rust

See Aya docs: https://aya-rs.dev/book/start/development/#who-aya-is-for

## Building this

```sh
$ rustup install stable
$ rustup toolchain install nightly --component rust-src
$ cargo install bpf-linker
$ cargo install cargo-generate
$ cargo generate --name hello-world-rs -d program_type=xdp https://github.com/aya-rs/aya-template
```

Once the initial project generated, compile it:

```
$ cd hello-world-rs/
$ cargo xtask build-ebpf
$ cargo build
```

## Running

Run the following task and `ping -c 1 0`

```
$ RUST_LOG=info cargo xtask run
    Finished dev [unoptimized + debuginfo] target(s) in 0.05s
[2023-09-14T08:26:00Z INFO  hello_world_rs] Waiting for Ctrl-C...
[2023-09-14T08:26:49Z INFO  hello_world_rs] received a packet
[2023-09-14T08:26:49Z INFO  hello_world_rs] received a packet

```