# CodeSamples

This repository contains code samples and utility scripts for software
development. The C++ projects are built with GCC and intended for Linux.

## C++ projects

### `SimpleEchoServer/`

A multithreaded TCP echo server written with POSIX sockets and pthreads. It
handles each client in a worker thread, echoes received text, and shuts down on
`SIGINT` or `SIGTERM`. It listens on port `22000` by default; use `-p` to
choose another port and `-h` to show usage.

```bash
make -C SimpleEchoServer
./SimpleEchoServer/SimpleEchoServer -p 22000
```

The server binds to all IPv4 interfaces, so run it only where you intend to
accept network connections.

### `TestProject/`

A C++ standard-library demonstration program covering threads, promises and
futures, lambdas, `shared_ptr`/`weak_ptr`, container algorithms, and the
`map`, `set`, and `unordered_map` containers.

```bash
make -C TestProject
./TestProject/TestApp
```

### `ipv4generator/`

Generates all valid IPv4 addresses that can be formed by inserting separators
into a string of 4–12 digits. Octets must be in the range 0–255 and cannot
contain leading zeroes.

```bash
make -C ipv4generator
./ipv4generator/ipv4generator 25525511135
```

## Shell scripts

| Script | Description |
| --- | --- |
| `scripts/replace_tabs.sh` | Replaces tab characters in a file with spaces. Accepts an optional tab width (default: 4) and creates a `.bak` backup. |
| `scripts/trim_trailing_whitespace.sh` | Removes trailing spaces and tabs from one or more files. Use `-b` to preserve a backup. |
| `scripts/sync_dirs.sh` | Mirrors a source directory into a destination using `rsync`. Supports dry runs, preserving destination-only files with `--no-delete`, and reversing the source/destination roles. By default, destination-only files are deleted. |
| `scripts/webserver-simulator.sh` | Runs a simple `nc`-based HTTP response simulator (port 8080 by default) that displays host, uptime, and load information. Accepts an optional port; intended for testing, not production use. |
| `scripts/net-forwarding.sh` | Adds `iptables` rules for LAN-to-WAN forwarding and NAT. Accepts LAN and WAN interface names; defaults to `enp0s25` and `wlo1`. It requires root and changes firewall rules. Kernel IPv4 forwarding must be enabled separately. |
| `scripts/lan-device-scan.sh` | Uses ARP requests through `arp-scan` to discover responsive devices on a directly connected IPv4 network. It does not scan ports. See [Discovering devices on a local network](#discovering-devices-on-a-local-network) below. |

## Discovering devices on a local network

On Ubuntu, install `arp-scan` and run the LAN inventory script with `sudo`:

```bash
sudo apt install arp-scan
sudo ./scripts/lan-device-scan.sh
```

By default, the script scans the directly connected IPv4 network on the
interface used by the default route. To select a different interface, pass its
name, for example `sudo ./scripts/lan-device-scan.sh wlp2s0`. The output lists
responsive devices' IP and MAC addresses and, when available, the vendor.
`arp-scan` uses local ARP discovery; the script does not probe ports.

Run this only on a network you own or are authorized to inventory. No single
scan can guarantee that every connected device will respond: sleeping devices,
client isolation, separate VLANs, and other network policies can hide devices.
For reconciliation, also check your router's DHCP/client list.
