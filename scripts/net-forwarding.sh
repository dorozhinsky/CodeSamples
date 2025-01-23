#!/bin/bash

#If you have 2 NICs on a Linux box, both configured with IP's you don't have to add a route from one network to another. That will be done automatically.
#Add a default gateway address on the WAN NIC. Do not do this in the configuration of the LAN NIC.
#Then enable forwarding in the kernel:
#echo 1 >> /proc/sys/net/ipv4/ip_forward
#To make it auto-set this value on boot uncomment this line in/etc/sysctl.conf
#net.ipv4.ip_forward=1

set -euo pipefail

WAN_ETH="wlo1"
LAN_ETH="enp0s25"

if [ $# -lt 2 ]; then
    echo "The default values for LAN and WAN interfaces will be used"
else
    LAN_ETH=$1
    WAN_ETH=$2
fi

echo "LAN interface = ${LAN_ETH}, WAN interface = ${WAN_ETH}"

# Always accept loopback traffic
iptables -A INPUT -i lo -j ACCEPT

# We allow traffic from the LAN side
iptables -A INPUT -i ${LAN_ETH} -j ACCEPT

######################################################################
#
#                         ROUTING
#
######################################################################

# Allow established connections
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
# Masquerade.
iptables -t nat -A POSTROUTING -o ${WAN_ETH} -j MASQUERADE
# forwarding
iptables -A FORWARD -i ${WAN_ETH} -o ${LAN_ETH} -m state --state RELATED,ESTABLISHED -j ACCEPT
# Allow outgoing connections from the LAN side.
iptables -A FORWARD -i ${LAN_ETH} -o ${WAN_ETH} -j ACCEPT

echo "Interface forwarding was enabled"

