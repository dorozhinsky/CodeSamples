#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<EOF
Usage: sudo $0 [INTERFACE]

Discover devices on the directly connected IPv4 network for INTERFACE using
ARP requests. If omitted, the interface for the default IPv4 route is used.
This discovers devices only on the local network; it does not scan ports.

Install the required tool on Ubuntu with:
  sudo apt install arp-scan
EOF
}

if [[ ${1:-} == "-h" || ${1:-} == "--help" ]]; then
  usage
  exit 0
fi

if [[ $# -gt 1 ]]; then
  echo "Error: expected zero or one network interface." >&2
  usage >&2
  exit 2
fi

if [[ $EUID -ne 0 ]]; then
  echo "Error: run this script with sudo so arp-scan can send ARP requests." >&2
  exit 1
fi

if ! command -v ip >/dev/null 2>&1; then
  echo "Error: the ip command was not found; install iproute2 and retry." >&2
  exit 1
fi

interface="${1:-}"
if [[ -z "$interface" ]]; then
  interface="$(ip -4 route show default | awk 'NR == 1 { for (i = 1; i <= NF; i++) if ($i == "dev") { print $(i + 1); exit } }')"
fi

if [[ -z "$interface" ]]; then
  echo "Error: no default IPv4 interface found; specify one explicitly." >&2
  exit 1
fi

if ! ip link show dev "$interface" >/dev/null 2>&1; then
  echo "Error: network interface '$interface' was not found." >&2
  exit 1
fi

if ! command -v arp-scan >/dev/null 2>&1; then
  echo "Error: arp-scan was not found. Install it with: sudo apt install arp-scan" >&2
  exit 1
fi

echo "Discovering devices on the local network via interface '$interface'..."
exec arp-scan --interface="$interface" --localnet
