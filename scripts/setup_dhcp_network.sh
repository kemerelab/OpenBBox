#!/bin/bash

IFACE_INTERNET_NETWORK=$1
IFACE_PRIVATE_NETWORK=$2

IP_ADDRESS='192.168.1'
IP_MASK='255.255.255.0'

# update repo
sudo apt-get update

sudo echo "Setting up interfaces"

# Loop interface
sudo echo "auto lo" > /etc/network/interfaces
sudo echo "iface lo inet loopback" >> /etc/network/interfaces

# IFACE_INTERNET_NETWORK Setup
sudo echo "auto $IFACE_INTERNET_NETWORK" >> /etc/network/interfaces
sudo echo "iface $IFACE_INTERNET_NETWORK inet dhcp" >> /etc/network/interfaces

# IFACE_PRIVATE_NETWORK Setup
sudo echo "auto $IFACE_PRIVATE_NETWORK" >> /etc/network/interfaces
sudo echo "iface $IFACE_PRIVATE_NETWORK inet static" >> /etc/network/interfaces
# Config
sudo echo "address $IP_ADDRESS.1" >> /etc/network/interfaces
sudo echo "netmask $IP_MASK" >> /etc/network/interfaces
sudo echo "network $IP_ADDRESS.0" >> /etc/network/interfaces
sudo echo "broadcast $IP_ADDRESS.255" >> /etc/network/interfaces

sudo /etc/init.d/networking restart

# Enabling ip forward
sudo echo "net.ipv4.ip_forward=1" >> /etc/sysctl.conf
sudo sysctl -w net.ipv4.ip_forward=1

# Set Forward configuration at boot
sudo echo "#!/bin/sh -e" > /etc/rc.local
sudo echo "" >> /etc/rc.local
sudo echo "/sbin/iptables -P FORWARD ACCEPT" >> /etc/rc.local
sudo echo "/sbin/iptables -A FORWARD -i $IFACE_PRIVATE_NETWORK -j ACCEPT" >> /etc/rc.local
sudo echo "/sbin/iptables --table nat -A POSTROUTING -o $IFACE_INTERNET_NETWORK -j MASQUERADE" >> /etc/rc.local
sudo echo "" >> /etc/rc.local
sudo echo "exit 0" >> /etc/rc.local

sudo iptables -P FORWARD ACCEPT
sudo iptables -A FORWARD -i $IFACE_PRIVATE_NETWORK -j ACCEPT
sudo iptables --table nat -A POSTROUTING -o $IFACE_INTERNET_NETWORK -j MASQUERADE

# Installing DHCP SERVER
sudo apt-get install --fix-missing -y isc-dhcp-server

sudo echo "ddns-update-style none;" > /etc/dhcp/dhcpd.conf
sudo echo "option domain-name \"dhcpserver.localhost\";" >> /etc/dhcp/dhcpd.conf
sudo echo "option domain-name-servers 8.8.8.8;" >> /etc/dhcp/dhcpd.conf
sudo echo "default-lease-time 600;" >> /etc/dhcp/dhcpd.conf
sudo echo "max-lease-time 7200;" >> /etc/dhcp/dhcpd.conf
sudo echo "authoritative;" >> /etc/dhcp/dhcpd.conf
sudo echo "log-facility local7;" >> /etc/dhcp/dhcpd.conf
sudo echo "" >> /etc/dhcp/dhcpd.conf
sudo echo "subnet $IP_ADDRESS.0 netmask $IP_MASK {" >> /etc/dhcp/dhcpd.conf
sudo echo "  range $IP_ADDRESS.2 $IP_ADDRESS.254;"  >> /etc/dhcp/dhcpd.conf
sudo echo "  option subnet-mask $IP_MASK;"  >> /etc/dhcp/dhcpd.conf
sudo echo "  option broadcast-address $IP_ADDRESS.255;"  >> /etc/dhcp/dhcpd.conf
sudo echo "  option routers $IP_ADDRESS.1;"  >> /etc/dhcp/dhcpd.conf
sudo echo "}"  >> /etc/dhcp/dhcpd.conf

# Limit DHCP Server to IFACE_PRIVATE_NETWORK interface
sudo echo "INTERFACES=\"$IFACE_PRIVATE_NETWORK\"" > /etc/default/isc-dhcp-server

sudo service isc-dhcp-server restart

echo "Done! If not working try restart the computer..."