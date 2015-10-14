#!/usr/bin/python
#handles the music page, shows current song info, and controls playback

import dispMod as lcd
import bluePlayer as bt
import time
import threading

newTrack = False
newPosition = False
	
def setNewTrack():
	global newTrack
	newTrack = True
	
def setNewPosition():
	global newPosition
	newPosition = True
	
def checkNewPosition():
	global newPosition
	if newPosition:
		newPosition = False
		return True
	else:
		return False
	
def checkNewTrack():
	global newTrack
	if newTrack:
		newTrack = False
		return True
	else:
		return False

		
class Page(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		self.exitNow = False
		
		self.btnPollRate = .1				#the rate at which the display module buttons are checked
		self.prevButton = None				
		self.pageExitFlag = False			#lets the system know when the user pressed the menu button
		self.pageExitTimer = 0
		self.pageExitThreshold = 1			#time to hold the menu button to exit the page
		self.scrollSpeed = .15
		
		self.artist = None
		self.track = None
		self.formattedTrack = None
		self.prevTrack = None
		self.prevStatus = None
		self.connected = False
		self.prevConnected = True
		
		self.duration = 0					#the current song length/duration
		self.updateOffset = True			#if it's time to update the time remaining
		self.currentPositon = 0				#the current song position
		self.positionOffset = 0				#the current position offset
		self.offsetUpdateTimer = 0			
		self.prevRemaining = ""
		self.newRemaining = ""
		self.toggleTimeBlink = False		#if it's time to blink the time remaining
		self.lastTimeBlink = False			#holds the last blink status (blink on or off)
		self.remainingTime = 0
		self.timeBlinkTimer = 0
	
		self.btThread = bt.BluePlayer()
		self.BtnMon = lcd.BtnChecker(self.btnPollRate)
		self.musicScroll = lcd.LcdScroll("", self.scrollSpeed, 0, 16, 0)
		self.btScroll = lcd.LcdScroll("", self.scrollSpeed, 0, 16, 1)
		
		lcd.lcdClear()
		
		print("music page started")
		

	#runs the page, call this continuously for smooth operation
	def run(self):
		while(1):
			if self.exitNow:
				return
		
			#if self.btThread.player == None or self.btThread.getConnected() == False:		
			if self.btThread.getConnected() == False:
				#if we are connected to the phone
				self.connected = False
			else:
				self.connected = True
				
			if not self.connected:
				if self.prevConnected == True:
					self.prevConnected = False
					lcd.lcdClear()
					lcd.lcdSetCursor(0, 0)
					lcd.lcdWrite("  Disconnected  ")
					self.prevStatus = None
					self.prevTrack = None
					self.btScroll.updateOutput("Please activate your phone's bluetooth and then connect to Carbt.")
					
				self.btScroll.scroll()
				self.__processBtn(self.BtnMon.update())
			else:
				if self.prevConnected == False:
					self.prevConnected = True
					lcd.lcdClear()
					
				self.__musicData()
				self.__timeRemaining()
				self.__status()
				self.__processBtn(self.BtnMon.update())
			
		
	#gets the current song data, formats, and displays it
	def __musicData(self):
		if checkNewTrack():				#if the bluetooth handler got new track data
			self.prevTrack = self.formattedTrack
			
			#format the track data into "artist - title" form
			self.formattedTrack = self.btThread.getArtist() + " - " + self.btThread.getTitle()
			
			#replace any non ascii characters with a "?"
			self.formattedTrack = ''.join([i if ord(i) < 128 else '?' for i in self.formattedTrack])
			
			if self.prevTrack != self.formattedTrack:			#if we are playing a new track
				if self.musicScroll == None:					#if this is the first ever time we got a new track
					#set up a new text scroller object
					self.musicScroll = lcd.LcdScroll(self.formattedTrack, self.scrollSpeed, 0, 16, 0)
				else:
					#else, just update the current text scroller object
					self.musicScroll.updateOutput(self.formattedTrack)
					
				self.duration = self.btThread.getDuration()		#get the new track's duration
				
		self.musicScroll.scroll()

		
	#formats a seconds input into MM:SS form, and returns it
	def __printRemaining(self, input):
		if input < 0:
			input = 0
		
		self.prevRemaining = self.newRemaining
		minutes = int(input / 60)
		seconds = str(int(input - (minutes * 60))).zfill(2)
		self.newRemaining = "-" + str(minutes) + ":" + seconds
		
		#check if longer first to prevent flashing
		
		if len(self.newRemaining) < len(self.prevRemaining):
			lcd.lcdSetCursor(0, 1)
			for i in self.prevRemaining:
				lcd.lcdWrite(" ")
			
		lcd.lcdSetCursor(0, 1)
		lcd.lcdWrite(self.newRemaining)
		
		
		
	def __timeRemaining(self):
		if self.duration > 0:		#make sure a track has been loaded
			if checkNewPosition():			#if the bluetooth handler got an updated track position
				self.currentPositon = self.btThread.getPosition()	#get the new position
				self.positionOffset = 0								#reset the position offset since we just synchronized
				self.updateOffset = False
				self.offsetUpdateTimer = time.time()
				self.remainingTime = self.duration - self.currentPositon
				self.__printRemaining(self.remainingTime)
			
			if self.btThread.getStatus() == "playing":
				if self.updateOffset:
					self.positionOffset = self.positionOffset + 1
					self.remainingTime = self.duration - (self.currentPositon + self.positionOffset)
					self.__printRemaining(self.remainingTime)
					self.offsetUpdateTimer = time.time()
					self.updateOffset = False
				else:
					currentTime = time.time()		#get the current time
					
					#if it has been at least 1 second since the last position offset update
					if currentTime >= (self.offsetUpdateTimer + 1):
						self.updateOffset = True
			else:
				if self.toggleTimeBlink:
					if self.lastTimeBlink:
						self.__printRemaining(self.remainingTime)
						self.lastTimeBlink = False
					else:
						lcd.lcdSetCursor(0, 1)
						lcd.lcdWrite("       ")
						self.lastTimeBlink = True
					self.timeBlinkTimer = time.time()
					self.toggleTimeBlink = False
				else:
					currentTime = time.time()		#get the current time
					
					#if it has been at least 1 second since the last position offset update
					if currentTime >= (self.timeBlinkTimer + .5):
						self.toggleTimeBlink = True
				
				
	#prints the current plaback status on the bottom right of the display			
	def __status(self):		
		currentStatus = self.btThread.getStatus()
	
		if self.prevStatus != currentStatus:
			output = None
		
			if currentStatus == "playing":
				output = "Playing"
			else:
				output = " Paused"

			lcd.lcdSetCursor((16 - len(output)), 1)
			lcd.lcdWrite(output)
			self.prevStatus = currentStatus
	
	
	#processes button presses
	def __processBtn(self, button):
		if button != None:
			if button != self.prevButton:
				self.prevButton = button
		
				if self.connected:			#only check these buttons when connected
					if button == 1:					#play/pause
						#if the button was pressed, we need to toggle the play/pause status
						if self.btThread.getStatus() == "playing":
							self.btThread.pause()
						else:
							self.btThread.play()
					if button == 2:					#prev
						self.btThread.previous()
					if button == 3:					#next
						self.btThread.next()
						
				if button == 0:					#no buttons pressed
					if self.pageExitFlag:
						self.pageExitFlag = False
						self.exitNow = True
				if button == 4:					#mode/menu
					self.pageExitFlag = True