#!/usr/bin/python
#module used to control the display module
#might also monitor the shutdown gpio maybe

import serial
import settings
import time
from time import sleep

BTN_READ_REG = 0x01
LDR_READ_REG = 0x02
LED_WRITE_REG = 0x04
CONTRAST_WRITE_REG = 0x05
CURSOR_WRITE_REG = 0x06
START_LCD_REG = 0x07
STOP_LCD_REG = 0x08
CLEAR_LCD_REG = 0x09

brightness = None
contrast = None

ser = serial.Serial('/dev/ttyAMA0', 4800, timeout=None)

def init():
	ser.open()
	global brightness
	global contrast
	brightness = settings.saveData['brightness']
	contrast = settings.saveData['contrast']
	setBrightness(brightness)
	lcdSetContrast(63)
	lcdClear()
	sleep(.20)
	lcdSetContrast(contrast)
	sleep(.20)

#writes a string of text to the lcd
def lcdWrite(text):					
	ser.write(chr(START_LCD_REG))		#tell the device it is going to write something to the lcd
	
	try:
		output = int(text)
		ser.write(chr(text))			#send it to the display as a number value (like 0x00)
	except ValueError:
		if len(text) <= 16:				#make sure text is 16 chars or less for sanity reasons
			ser.write(text)				#write the text as it is out to the display
			
	# if isinstance(text, str):			#if the text is a string
		# if len(text) <= 16:				#make sure text is 16 chars or less for sanity reasons
			# ser.write(text)				#write the text as it is out to the display
	# else:								#if it's a number or something
		# ser.write(chr(text))			#send it to the display as a number value (like 0x00)
		# #this is needed for custom character writing
		
	ser.write(chr(STOP_LCD_REG))

#sets the lcd cursor position, needs column (0-15) and row (0-1) values
def lcdSetCursor(x, y):
	if x <= 15 and y <= 1:
		ser.write(chr(CURSOR_WRITE_REG))
		output = y << 7 | x
		ser.write(chr(output))

#clears the lcd of all text
def lcdClear():
	ser.write(chr(CLEAR_LCD_REG))

#sets the led contrast, needs a value from 0 to 63
def lcdSetContrast(contrast):
	ser.write(chr(CONTRAST_WRITE_REG))
	ser.write(chr(contrast))
	
#add text centering

#and text scrolling

#sets the overall display module LED brightness (lcd and buttons), needs a value from 0 to 255
def setBrightness(brightness):
	ser.write(chr(LED_WRITE_REG))
	ser.write(chr(brightness))

#gets the ambient light level from the device's LDR, returns 0-1024
def getLdr():
	ser.write(chr(LDR_READ_REG))
	#read two bytes from the display module
	#I can't use sr.read(2) because it pisses off ord()
	response1 = ser.read()
	response2 = ser.read()
	#combine the two bytes into a 10 bit value (0 - 1023)
	#output = (ord(response1) << 2) + (ord(response2) >> 6)
	output = (ord(response2) << 8) + ord(response1)
	return output;


#gets the last pressed button from the device, returns in index as an int (1-4)
def getButton():
	ser.write(chr(BTN_READ_REG))		#ask the device for the last pressed button(s)
	response = ser.read()
	response = ord(response)
	if (response & 0x08) == 8:
		return 4
	elif (response & 0x01) == 1:
		return 1
	elif (response & 0x02) == 2:
		return 2
	elif (response & 0x04) == 4:
		return 3
	else:
		return 0
		
		
#returns s string of text with empty space padding for centering, needs the start and stop columns (for width calc)
def centerText(text, x1, x2):
	width = x2 - x1
	padding = (width - len(text)) // 2				#get the padding size as an int, the lcd doesn't like floats
	
	if (width - len(text)) % 2 == 0:							#if the padding size is even
		return (" " * padding) + text + (" " * padding)		#return it normally
	else:													
		return (" " * (padding + 1)) + text + (" " * padding)	#else, add one to the first padding section
	

#used to check the buttons on a regular delay, to prevent spamming the device
class BtnChecker(object):

	#give it the time you want to wait in between updates
	def __init__(self, pollRate):
		self.pollRate = pollRate
		self.waiting = False
		self.buttonTimer = 0
		
	def update(self):
		if not self.waiting:					#if we are not waiting for the next time we can check the buttons
			self.waiting = True					#we are waiting for the next time to update the buttons now
			self.buttonTimer = time.time()
			return getButton()
		else:
			if time.time() >= (self.buttonTimer + self.pollRate):
				#if we have waited long enough
				self.waiting = False
			return None		
			
#it's like lcdscroll, but it only scrolls/slides in/out one string of text at a time, from a specified direction		
class LcdScrollOneshot(object):

	#this class needs a string to display, the speed in ms, start and stop column, row, and if text should be centered
	def __init__(self, text, speed, x1, x2, y, centered):
	
		self.x1 = x1
		self.x2 = x2
		self.y = y
		self.centered = centered
		self.scrolling = False
		
		if self.centered:
			self.text = centerText(text, self.x1, self.x2)
			lcdSetCursor(self.x1, self.y)		#set the cursor to the beginning as usual		
			lcdWrite(self.text)	
		else:
			self.text = text
			lcdSetCursor(self.x1, self.y)		#set the cursor to the beginning as usual		
			lcdWrite(self.text + (" " * (self.x2 - len(self.text))))	
			
		self.speed = speed
		self.separation = 4
		self.currentPosition = 0	#current scroll iteration position
		self.waiting = False
		
	def getScrolling(self):
		return self.scrolling
		
	def updateOutput(self, first, second, toLeft):
		#ok so basically
		#add the new text we got to the current text with a separator
		#then
		#set it to scroll in from the specific direction
		#do not do anything yet
		self.first = first
		self.second = second
		self.toLeft = toLeft
		
		if self.centered:
			self.first = centerText(self.first, self.x1, self.x2)
			self.second = centerText(self.second, self.x1, self.x2)
			
			if self.toLeft:							#if the text is moving left <----
				self.text = self.first + " " + self.second
				self.currentPosition = 0	#current scroll iteration position
			else:											#else, text is moving right ---->
				self.text = self.second + " " + self.first
				self.currentPosition = len(self.text) - self.x2	#current scroll iteration position
		else:
			if toLeft:
				self.second = self.second + (" " * (self.x2 - len(self.newText)))
				self.text = self.first + (" " * (self.x2 - len(self.first))) + self.second
				self.currentPosition = 0	#current scroll iteration position
			else:
				self.second = self.second + (" " * (self.x2 - len(self.second)))
				self.text = self.second + self.first + (" " * (self.x2 - len(self.first)))
				self.currentPosition = len(self.text) - self.x2	#current scroll iteration position
		
		
		self.scrolling = True
		self.waiting = False
		
	def scroll(self):
		#keep calling this until it returns false
		#it is used to scroll the next text in on the lcd
		#that's pretty much it
		
		if not self.waiting:					#if we are not waiting for the next scroll iteration
			lcdSetCursor(self.x1, self.y)		#set the cursor to the beginning as usual		
			output = self.text[self.currentPosition:self.x2 + self.currentPosition] #
			lcdWrite(output)					#send the formatted string to the display
			self.waiting = True					#we are now waiting for the next scroll iteration
			self.startWait = time.time()		#get the time we started waiting
			
			if self.toLeft:				#move the text left <----
				self.currentPosition += 1
			
				if self.currentPosition > (len(self.text) - len(self.second)):
					self.scrolling = False
					return False
				else:
					return True
			
			else:		#else, move the text right ---->
				self.currentPosition -= 1

				if self.currentPosition < 0:
					self.scrolling = False
					return False
				else:
					return True
			
		else:
			currentTime = time.time()			#get the current time
			if currentTime >= (self.startWait + self.speed):
				#if we have waited long enough, we are ready to start scrolling
				self.waiting = False
				self.started = True
				return True
		
#class used to scroll text on the lcd
class LcdScroll(object):

	#this class needs a string to scroll, the speed in ms, start and stop column, and row
	def __init__(self, text, speed, x1, x2, y):
		self.text = text
		self.speed = speed
		self.x1 = x1
		self.x2 = x2
		self.y = y
		
		self.separation = 4
		self.currentPosition = 0	#current scroll iteration position
		self.beginDelay = 2			#wait for 2 seconds after showing the beginning of the text
		self.waiting = False
		self.started = False
		
		if len(text) <= 16:			#check if scrolling is even necessary
			self.scrollNeeded = False
		else:
			self.scrollNeeded = True
		
	#updates the text to be scrolled
	def updateOutput(self, text):
		self.text = text
		self.currentPosition = 0
		self.started = False
		self.waiting = False
		
		if len(text) <= 16:			#check if scrolling is even necessary
			self.scrollNeeded = False
		else:
			self.scrollNeeded = True
		
	#all in one function used to scroll text on the display, call this continuously for smooth operation
	def scroll(self):
		if self.scrollNeeded:
			if not self.started:						#if the scrolling has not been started yet
				#run the first-time scroll procedure, it shows the beginning of the text to the user for some time
				if not self.waiting:					#if we are not yet waiting for the beginning
					lcdSetCursor(self.x1, self.y)		#set the cursor to the specified position
					lcdWrite(self.text[:self.x2])		#write the beginning of the text to the specified limit
					self.waiting = True					#we are now waiting
					self.startWait = time.time()		#get the time we started waiting
				else:									#else if we are currently waiting
					currentTime = time.time()			#get the current time
					if currentTime >= (self.startWait + self.beginDelay):
						#if we have waited long enough, we are ready to start scrolling
						self.waiting = False
						self.started = True
			else:										#if we are currently scrolling
				if not self.waiting:					#if we are not waiting for the next scroll iteration
					lcdSetCursor(self.x1, self.y)		#set the cursor to the beginning as usual
					self.currentPosition += 1
					output = None
					
					#if we are at the end of the string "extension" (more info below)
					if self.currentPosition > (len(self.text) + self.separation):
						self.currentPosition = 0		#reset the current position
					
					#if we are at the end of the string
					if self.currentPosition > (len(self.text) - self.x2):
						#we will need to wrap the beginning of the text string to the end
						#so it looks like it loops continuously
						
						#add a separation and the same string to the end
						output = self.text + (" " * self.separation) + self.text
						
						#get the displayable substring of the above out
						output = output[self.currentPosition:self.x2 + self.currentPosition]	
					else:
						#else, just get the displayable substring out
						output = self.text[self.currentPosition:self.x2 + self.currentPosition]
					
					lcdWrite(output)					#send the formatted string to the display
					self.waiting = True					#we are now waiting for the next scroll iteration
					self.startWait = time.time()		#get the time we started waiting
				else:
					currentTime = time.time()			#get the current time
					if currentTime >= (self.startWait + self.speed):
						#if we have waited long enough, we are ready to start scrolling
						self.waiting = False
						self.started = True
		else:											#else if the text doesn't need to be scrolled (<= 16)
			if not self.started:						#only print it once
				lcdSetCursor(self.x1, self.y)			#set the cursor to the specified position
				
				#clear the entire section
				#for x in range(self.x1, self.x2):
				lcdWrite(" " * (self.x2 - self.x1))
				
				lcdSetCursor(self.x1, self.y)
				lcdWrite(self.text)						#print the entire string
				self.started = True						#we have started