#!/bin/bash 

NUM_DEVICES=0			#number of paired devices
CONNECTING=false
CONNECTED=false

#connects the first paired device it can
connect() {
	while $CONNECTING; do				#only try to connect if there are no devices currently connected
		while read -r macAdd ; do				#for each mac address (paired device) returned by the grep below
		#while [[ read -r macAdd && "$CONNECTED" = false ]]; do
			if [[ "$CONNECTED" = false ]]; then
				logger "[$(basename $0)] Found: $macAdd"
				let NUM_DEVICES+=1
						
				bluetoothctl <<< "connect $macAdd"			#try to connect to the current device
				sleep 2										#wait for the connection request to process
						
				checkConnection $macAdd						#check the connection status
				
				if [[ $? == 0 ]]; then
					logger "[$(basename $0)] No connection, trying another device"
					CONNECTED=false
				else
					logger "[$(basename $0)] Connected now!"
					CONNECTING=false
					CONNECTED=true
				fi
			fi
			
		done < <(bluetoothctl <<< "paired-devices" | awk '/Device/{print $2}' | grep -oEi -m 5 "([0-9A-F]{2}[:-]){5}([0-9A-F]{2})")
	done

	#echo "Number of paired devices: " $NUM_DEVICES
}

#checks the connection status and returns if the device is connected, needs the mac address of the device to check
checkConnection() {
	if bluetoothctl <<< "info $1" | grep -qi "Connected: no"; then
		return 0
	else
		return 1
	fi
}

while true; do

	#check if there are any devices already connected
	if [[ -n $(hcitool con | grep -oEi -m 5 "([0-9A-F]{2}[:-]){5}([0-9A-F]{2})") ]]; then		#if there was a mac address in hcitool con's output
		#there is something connected, don't try to connect something else
		logger "[$(basename $0)] Already connected."
		CONNECTING=false
		CONNECTED=true
	else
		#nothing connected, you're free to go
		logger "[$(basename $0)] Not connected yet, going to try now"
		CONNECTING=true
		CONNECTED=false
	fi

	connect
	
	#also check for that module-loopback.c: Sample rates too different, not adjusting (happens if the phone goes too far?)
	#or just monitor the pactl status or something

	sleep 1
done

