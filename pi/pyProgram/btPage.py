#!/usr/bin/python
#handles user-side bluetooth control, pairing, unpairing, connection status, etc

import dispMod as lcd
import time
import threading
import thread
import settings
import bluePlayer as btPlayer
import subprocess

class Page(threading.Thread):

	def __init__(self):
		threading.Thread.__init__(self)
	
		self.scrollSpeed = .15
		self.btnPollRate = .05				#the rate at which the display module buttons are checked
		self.prevButton = None				
		self.pageExitFlag = False			#lets the system know when the user pressed the menu button
		self.pageExitTimer = 0
		self.pageExitThreshold = 1			#time to hold the menu button to exit the page
		self.exitNow = False
		self.BtnMon = lcd.BtnChecker(self.btnPollRate)
		
		self.connected = False
		self.prevConnected = None
		self.checkSignalQuality = True
		self.signalQualityTimer = 0
		self.signalQualityDelay = 10
		self.signalQuality = None
		self.prevSignalQuality = None
		
		#self.BtPlayerThread = btPlayer.BluePlayer()
		self.BtPlayerThread = btPlayer.BluePlayer()
		#self.BtAgent = btPlayer.BlueAgent("0000")
		self.ConnectedScroll = lcd.LcdScroll("", self.scrollSpeed, 0, 16, 0)
		self.BtScroll = lcd.LcdScroll("", self.scrollSpeed, 0, 16, 0)
		self.PairModeScroll = lcd.LcdScroll("", self.scrollSpeed, 0, 16, 1)
		
		self.pairModes = {0: "New Pair", 1: "Unpair"}
		self.currentPairSelection = 0
		self.PairingSlider = lcd.LcdScrollOneshot(self.pairModes[self.currentPairSelection], .05, 1, 14, 1, True)
		
		lcd.lcdClear()
		
		print("bluetooth page started")
		

	#runs the page, call this continuously for smooth operation
	def run(self):
		while(1):
			if self.exitNow:
				return
				
			self.__checkConnection()
			self.__processBtn(self.BtnMon.update())
				
			if self.connected:
				self.__dispConnected()
			
			
	def __dispConnected(self):
		self.ConnectedScroll.scroll()
		
		if self.prevSignalQuality != self.signalQuality:
			self.prevSignalQuality = self.signalQuality
			self.__showSignalQuality(self.signalQuality)
		
		if self.checkSignalQuality:
			thread.start_new_thread( self.__updateSignalQuality, () )
			self.__updateSignalQuality()
			self.signalQualityTimer = time.time()
			self.checkSignalQuality = False
		else:
			currentTime = time.time()		#get the current time
			
			#if it has been at least signalQualityDelay seconds since the last signal quality update
			if currentTime >= (self.signalQualityTimer + self.signalQualityDelay):
				self.checkSignalQuality = True
			

	def __showSignalQuality(self, input):
		output = None
		
		if input == None:
			output = "Signal: ---%  "
		else:	
			output = "Signal: " + str(int(float(input) / float(255) * 100)) + "%  "
		
		lcd.lcdSetCursor(0,1)
		lcd.lcdWrite(output)
				

	def __updateSignalQuality(self):
		macAdd = subprocess.check_output('hcitool con | grep -oEi -m 5 "([0-9A-F]{2}[:-]){5}([0-9A-F]{2})"', shell=True)
		macAdd = macAdd.rstrip()
		output = subprocess.check_output('hcitool lq ' + macAdd + ' | grep -Eo "[0-9]{1,4}"', shell=True)
		self.signalQuality = output.rstrip()
	
	
	def __checkConnection(self):
		if self.BtPlayerThread.player == None or self.BtPlayerThread.getConnected() == False:			
			#if we are not connected to the phone
			self.connected = False
		else:
			self.connected = True
			
		if self.connected != self.prevConnected:			#if the connection status just changed
			self.prevConnected = self.connected
			lcd.lcdClear()
			
			if self.connected:
				macAdd = subprocess.check_output('hcitool con | grep -oEi -m 5 "([0-9A-F]{2}[:-]){5}([0-9A-F]{2})"', shell=True)
				macAdd = macAdd.rstrip()
				
				deviceName = subprocess.check_output('hcitool name ' + macAdd, shell=True)
				deviceName = deviceName.rstrip()
				
				output = deviceName + " " + macAdd
				self.ConnectedScroll.updateOutput(output)
			else:
				lcd.lcdSetCursor(0,0)
				lcd.lcdWrite(lcd.centerText("No device", 0, 15))
				lcd.lcdSetCursor(0,1)
				lcd.lcdWrite(lcd.centerText("connected.", 0, 15))

	#processes button presses
	def __processBtn(self, button):
		if button != None:
			if button != self.prevButton:
				self.prevButton = button
						
				if button == 0:					#no buttons pressed, or just released the button
					if self.pageExitFlag:
						self.pageExitFlag = False
						self.exitNow = True
				if button == 4:					#mode/menu
					self.pageExitFlag = True