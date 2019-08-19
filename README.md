# Universal-Photon-Network-Controller
 Network Controller for Anycubic Photon
![1](https://github.com/Photonsters/Universal-Photon-Network-Controller/raw/master/platformIndependancy.png)

## Requirements

* CBD based printer with an accessible ethernet port. I am using Anycubic Photon
* A Computer running Windows, Mac or Linux. Preferably Windows or Mac if you are interested in just running the program without much hassle

## Installation

#### Windows

Uncompress all the files in the compressed archive to somewhere on the computer. Double click on the executable file to run it. It doesn't need administrative access or any special installation. It can be regarded as a portable application.

#### Mac

Mount the dmg file and drag the software to the Application directory. It should run without any problem. It has been tested to run fine down to Mac OS X 10.9 Maverick. It Might work on even older versions of Macs but no guarantees.

#### Linux

The program was tested on 64-bit version of Ubuntu 18.04.2 and OpenSUSE 42.3. But it might not work as the program needs wxWigets library installed. If you are on a Linux OS then it is advisable to install wxWigets and then compile the program from source. Even though it seems like a complicated and lengthy method it has a higher rate of success than just using the binaries as is.

## Usage

The interface of the program is quite intuitive and self-explanatory. However here are some details of some fields and buttons to make the usage go smoother.

1. "IP Address" - The IP address of the printer. The program remembers the last used IP
1. "Connect" Button - Clicking this button tries to connect to the printer. It will show an error message if the IP address doesn't exist or doesn't respond
1. "Settings" Button - This button opens the settings dialog box. Below are the description of the settings
 1. "Ping Timeout in millisecond" - The time for pinging the IP address to check if it exists or not.
 1. "Reply Timeout in millisecond" - The timeout for the software to decide if the IP is responding or not.
 1. "Polling Interval in millisecond" - When printing is in progress the software querries the printer at this interval to get the print status.
 1. "Connection Port" - The connection port if it different than the default value 3000
1. "Start" Button - Starts printing a selected file.
1. "Pause" Button - Pauses the current running print job. When the print is in the paused state the label of the button changes to "Resume"
1. "Stop" Button - Stops the current print job. 
1. "Delete" Button - Deletes the selected file.
1. "Refresh" Button - Refreshes the file list.
1. "Upload" Button - Upload file to the USB drive connected to the printer.
1. "Download" Button - Download the selected file from the printer to the computer.
1. The "Percent Done" progress bar shows the print progress.
1. The progress bar near the download button shows the file download or upload progress.

## Compiling

### Windows
1. Download wxWigets 3.1 from wxWigets website. Compile it for windows. Here is a video tutorial on compiling wxWigets with visual studio 2017 https://www.youtube.com/watch?v=EI2taYkErRg
1. Open the "PhotonNetworkTool.sln" and change the path for linker files and include files as shown in the tutorial video. It should compile without any errors after that.

1. If you are interested in compiling it under mingw i.e. GCC for Windows then first compile the wxWigets library with mingw.
1. Open "PhotonNetworkToolWindows.cbp" and modify the linker and include paths to reflect your installation of wxWigets. After this point, it should compile without any error

### Mac
1. You have to install XCode or just the GCC toolchain before attempting to compile the program under Mac.
1. Download wxWigets 3.1 from wxWigets website. Navigate to the directory where you uncompressed the library and issue the following command _./configure --with-cocoa --macosx-version_min=10.8 --enable-macosx_arch=x86_64_ . After it is finished issue the _make_ command. After it is done issue _sudo make install_ . It will ask you for your password to install the just compilled library in your computer.
1. After that navigate to the source folder of PhotonNetworkController and issue the command _make -f PhotonNetworkToolMac.mak release_ . It will make the release version of the program. The compiled program can be found under _<current_Directory>/Mac/x64/Release_

### Linux
1. You have to install the GCC toolchain before attempting to compile the program under Linux.
1. Download wxWigets 3.1 from wxWigets website. Navigate to the directory where you uncompressed the library and issue the following command _./configure_ . After it is finished issue the _make_ command. After it is done issue _sudo make install_ . It will ask you for your password to install the just compiled library in your computer.
1. After that navigate to the source folder of PhotonNetworkController and issue the command _make -f PhotonNetworkToolLinux.mak release_ . It will make the release version of the program. The compiled program can be found under _<current_Directory>/Unix/x64/Release_
1. Alternatively one can use Code::Bloks IDE to compile the program under Linux. Open the project file "PhotonNetworkToolLinux.cbp" to compile the program using the IDE.

