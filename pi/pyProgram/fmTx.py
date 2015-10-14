#!/usr/bin/python
#module used to control the qn8027 fm transmitter

import smbus
import settings
import time

currentFreq = 0
maxFreq = 10790
minFreq = 7610

bus = smbus.SMBus(1)    #we are using i2c port 1
DEVICE_ADDRESS = 0x2C      #7 bit address (will be left shifted to add the read write bit)

SYSTEM_REG = 0x00		#system register
CH1_REG = 0x01			#register that the lower 8 bits of the frequency goes in
GPLT_REG = 0x02
RDS_REG = 0x12

SWRST = 0x80
RECAL = 0x40
RDSEN = 0x80
RDSRDY = 0x04

CH0_MASK = 0x03
CH1_MASK = 0xFF
TRANSMIT_MASK = 0x20
PAOFF_MASK = 0x30


#initializes the device and sets default values to it
def init():
	#TRY CONNECTING TO THE DEVICE FIRST


	global currentFreq
	currentFreq = settings.saveData['frequency']
	writeData(SYSTEM_REG, SWRST, SWRST)		#reset all registers
	writeData(0x04, 0x03, 0x01)
	#writeData(0x11, 0xFF, 0xF0)
	recalibrate()
	writeData(GPLT_REG, PAOFF_MASK, PAOFF_MASK)		#tell the device that it shouldn't go into standby if there is no audio
	setFrequency(currentFreq)
	print("Transmitting on: " + str(getFrequency()))
	enableTransmit(True)


#I don't know if this actually helps anything, but it doesn't seem to hurt
def recalibrate():
	writeData(SYSTEM_REG, RECAL, RECAL)
	writeData(SYSTEM_REG, RECAL, 0x00)
	
	
#sets the frequency, give it a frequency formatted like 10310 for 103.1MHz, or 9890 for 98.9MHz
#make sure the frequency is an odd numbered 200KHz slice, and not something like 98.4MHz
def setFrequency(freq):
	freq = (freq - 7600) / 5	#convert the frequency to a 10 bit decimal
	#we need 10 bits because the device wants 2 bits to go in on register, and the other 8 in another register
	#it's strange but whatever
	partOne = freq >> 8		#shift it to the right 8 times, this leaves us with the first 2 bits
	partTwo = freq & 0xFF	#AND it with a mask covering the last 8 bits, leaving the last 8 bits
	
	#write out the parts to their specific registers
	writeData(SYSTEM_REG, CH0_MASK, partOne)
	writeData(CH1_REG, CH1_MASK, partTwo)


#returns the current frequency set on the device formatted like 10310 for 103.1MHz
def getFrequency():
	partOne = bus.read_byte_data(DEVICE_ADDRESS, SYSTEM_REG)	#get the first part out of the system register
	partOne = partOne & CH0_MASK	#mask out the two bits that we want
	partTwo = bus.read_byte_data(DEVICE_ADDRESS, CH1_REG)	#get the second part out, no need to apply any masks
	
	combined = (partOne << 8) | partTwo
	#shift the first part left two, and then OR them together, leaving a 10 bit value
	output = (combined * 5) + 7600		#convert the output to something readable
	return output
	

#enabled or disable FM transmission
def enableTransmit(enabled):
	if enabled:
		writeData(SYSTEM_REG, TRANSMIT_MASK, TRANSMIT_MASK)
	else:
		writeData(SYSTEM_REG, TRANSMIT_MASK, 0x00)
	

#writes specific bits of data to a specified register
def writeData(register, bitmask, data):
	currentData = bus.read_byte_data(DEVICE_ADDRESS, register)
	#get the data that is already in the specified register (for merging in new data)
	
	output = currentData & (~ bitmask)	#AND the current data with the NOTed bitmask
	output = output | (data & bitmask)	#AND the new data with the bitmask, and then OR it with the current output		
	
	bus.write_byte_data(DEVICE_ADDRESS, register, output)		#write out the data


# def nextFreq():


# def prevFreq():