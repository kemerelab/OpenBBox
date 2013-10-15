#!/bin/bash

if [ -e qt-linux-opensource-5.1.1-x86_64-offline.run ]; then
	echo "Downloaded QT Creator"
else
	echo "Downloading QT Creator"
	wget http://download.qt-project.org/official_releases/qt/5.1/5.1.1/qt-linux-opensource-5.1.1-x86_64-offline.run
fi

chmod +x qt-linux-opensource-5.1.1-x86_64-offline.run
./qt-linux-opensource-5.1.1-x86_64-offline.run

if [ -e /home/$USER/Qt5.1.1/Tools/QtCreator/bin/qtcreator ]; then
	sudo ln -s /opt/Qt5.1.1/Tools/QtCreator/bin/qtcreator /usr/bin/qtcreator
	echo "QT Installation successful!!!"

	if [ -e angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2 ]; then
		echo "Downloaded ARM QT Toolchain"
	else
		echo "Downloading ARM QT Toolchain"
		wget http://www.angstrom-distribution.org/toolchains/angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2
	fi

	if [ -e /usr/local/angstrom ]; then
		echo "Extracted ARM QT Toolchain"
	else
		echo "Extracting ARM QT Toolchain"
		sudo tar -C / -xjf angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2
	fi
	
	echo "Setting environment variables"
	. /usr/local/angstrom/arm/environment-setup

	echo "Installing libs"
	sudo apt-get install --fix-missing -y ffmpeg libavutil-dev libavformat-dev libavcodec-dev libavdevice-dev libavfilter-dev libswscale-dev \
	libsdl-image1.2-dev libsdl-mixer1.2-dev libexiv2-dev libtag1-dev libavcodec-extra-53 libavformat-extra-53 \
	libavdevice-extra-53 libavfilter-extra-2 libswscale-extra-2 libav-tools
	
else
	echo "Installation failed!!!"
fi

