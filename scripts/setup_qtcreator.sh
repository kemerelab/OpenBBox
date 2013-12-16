#!/bin/bash

VERSION='5.1'
PATCH='1'
INSTALLER=qt-linux-opensource-$VERSION.$PATCH-x86_64-offline.run

installQT(){

	if [ -e $INSTALLER ]; then
		echo "[-] Downloaded QT Creator"
	else
		echo "[-] Downloading QT Creator"
		wget http://download.qt-project.org/official_releases/qt/$VERSION/$VERSION.$PATCH/$INSTALLER
	fi

	chmod +x $INSTALLER
	./$INSTALLER

	if [ -e /home/$USER/Qt$VERSION.$PATCH/Tools/QtCreator/bin/qtcreator ]; then
		sudo ln -s /opt/Qt$VERSION.$PATCH/Tools/QtCreator/bin/qtcreator /usr/bin/qtcreator
		echo "[-] QT Installation successful"
	else
		echo "[!] Installation failed"
		echo "[!] Exiting..."
		return 1
	fi
}

armToolChain(){

	if [ -e angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2 ]; then
		echo "[-] Downloaded ARM QT Toolchain"
	else
		echo "[-] Downloading ARM QT Toolchain"
		wget http://www.angstrom-distribution.org/toolchains/angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2

		if [ -e angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2 ]; then
			echo "[-] Download successful"
		else
			echo "[!] Download failed"
			echo "[!] Exiting..."
			return 1
		fi
	fi

	if [ -e /usr/local/angstrom ]; then
		echo "[-] Extracted ARM QT Toolchain"
	else
		echo "[-] Extracting ARM QT Toolchain"
		sudo tar -C / -xjf angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2
	fi
	
	echo "[-] Setting environment variables"
	. /usr/local/angstrom/arm/environment-setup
}	

installLibs(){
	echo "[-] Installing libs"
	sudo apt-get install --fix-missing ffmpeg libavutil-dev libavformat-dev libavcodec-dev libavdevice-dev libavfilter-dev libswscale-dev \
	libsdl-image1.2-dev libsdl-mixer1.2-dev libexiv2-dev libtag1-dev libavcodec-extra-53 libavformat-extra-53 \
	libavdevice-extra-53 libavfilter-extra-2 libswscale-extra-2 libav-tools
}

#main function
while true; do
    read -p "[?] Do you wish to install the Qt$VERSION.$PATCH? " yn
    case $yn in
        [Yy]* ) installQT; break;;
        [Nn]* ) break;;
        * ) echo "[-] Please answer yes or no.";;
    esac
done

while true; do
    read -p "[?] Do you wish to install the Arm Toolchain? " yn
    case $yn in
        [Yy]* ) armToolChain; break;;
        [Nn]* ) break;;
        * ) echo "[-] Please answer yes or no.";;
    esac
done

while true; do
    read -p "[?] Do you wish to install the dependecies? " yn
    case $yn in
        [Yy]* ) installLibs; break;;
        [Nn]* ) break;;
        * ) echo "[-] Please answer yes or no.";;
    esac
done
