<Usage of 1-10 image tool for AmebaD>
1. Unzip AmebaD_ImageTool.zip
       - unzip AmebaD_ImageTool.zip


2. Check tool usage
       - ./AmebaD_ImageTool -help


3. Set AmebaD chip
Example:
./AmebaD_ImageTool -set chip AmebaD                       
command finish


4. Add Uart Device	
Example:
- ./AmebaD_ImageTool -add device /dev/ttyXXX
command finish
     

5. Set Baudrate
Example:
./AmebaD_ImageTool -set baudrate 1500000
command finish


6. Combine AmebaD three images
	- ./AmebaD_ImageTool -combine <file1> <offset1> <file2> <offset2> ...
	- ./AmebaD_ImageTool -combine km0_boot_all.bin 0x0000 km4_boot_all.bin 0x4000 km0_km4_image2.bin 0x6000
       (Tool will generate the Image_All.bin after combine command.)


7. Set image file path
       - ./AmebaD_ImageTool -set image /<Current_PATH>/Image_All.bin


8. Set image address
       - ./AmebaD_ImageTool -set address 0x08000000


9. Scan device port
Example:
./AmebaD_ImageTool -scan device
command finish
Automatically add to config
start ping
ping reboot uartburn
rate update 115200
no nak after reboot to uartburn
start ping
ping ok


10. Final check
Example:
./AmebaD_ImageTool -show                                  
command finish
Current Settings
	CPU      : AmebaD
	UART     : 1500000 bps parity:N flow control:False
	FW NAME  : /<Current_PATH>/Image_All.bin
	FW ADDR  : 0x8000000
	FW LEN   : 0
	OPTS     : keep data    :	True 
		   keep wifi cal:	True 
		   verification :	False
		   reset device :	False 
		   factory mode :	False 
		   save log     :	True
Port Settings
	PORT 0: active:True device:/dev/tty.XXX
	PORT 1: active:False device:NONE
	PORT 2: active:False device:NONE
	PORT 3: active:False device:NONE
	PORT 4: active:False device:NONE
	PORT 5: active:False device:NONE
	PORT 6: active:False device:NONE
	PORT 7: active:False device:NONE
	PORT 8: active:False device:NONE
	PORT 9: active:False device:NONE


11. Download image to AmebaD
Example: (Notice : please set download mode on AmebaD board before download command)
./AmebaD_ImageTool -download
log file = log_2021_03_12_13_58_46.txt
[100, 0, 0, 0, 0, 0, 0, 0, 0, 0]command finish

status: ['OK 58.4s', 'Off', 'Off', 'Off', 'Off', 'Off', 'Off', 'Off', 'Off', 'Off']
close log file
      
12. Check finish log