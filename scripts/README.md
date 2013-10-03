# Scripts

This section contains important scripts to help the developer to setup the development environment.

# Contents

## setup\_host\_network.sh

Setup an DHCP server network for the nodes providing Internet connection from another interface. The host must be at least 2 interfaces: one with Internet connection and another for the network with with the nodes. 

### Usage

    sh setup_host_network.sh $1 $2

$1 - interface with Internet access

$2 - Nodes network interface 

### Example

    sh setup_host_network.sh eth0 eth1
  
## setup\_qtcreator.sh

Download and install Qt Creator 2.8.1 and QT 5.1.1. Also install the cross compile for the beaglebone and the libav library.

### Usage

    sh setup_qtcreator.sh

Note: Don't install as sudo, this make permission issues 

## git_bash

Modification in the terminal bash to improve development by including information of git repository.

## INSTALL\_NODE\_REMOTE\_DEBUG

Describes how to setup a local development environment with cross compile and remote debug to the beaglebone using Qt Creator.