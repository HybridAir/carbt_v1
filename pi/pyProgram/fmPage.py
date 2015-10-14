#!/usr/bin/python
#handles the fm radio control page, shows current freq, and controls the freq

import dispMod as lcd
import fmTx
import time
import threading
import settings


class Page(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		
		self.btnPollRate = .05				#the rate at which the display module buttons are checked
		self.prevButton = None				
		self.exitNow = False
		self.BtnMon = lcd.BtnChecker(self.btnPollRate)
		
		self.currentFreq = settings.saveData['frequency']
		fmTx.setFrequency(self.currentFreq)
		
		lcd.lcdClear()
		lcd.lcdSetCursor(0,0)
		lcd.lcdWrite("  FM Frequency  ")
		lcd.lcdSetCursor(0,1)
		lcd.lcdWrite(251)
		self.__fmData()
		lcd.lcdSetCursor(15,1)
		lcd.lcdWrite(252)
		
		print("fm page started")
		
	#runs the page, call this continuously for smooth operation
	def run(self):
		while(1):
			if self.exitNow:
				return
		
			self.__processBtn(self.BtnMon.update())
		
	#gets the current fm tx freq, formats, and displays it
	def __fmData(self):	
		output = str(fmTx.getFrequency() * .01) + "MHz"
		lcd.lcdSetCursor(1,1)
		lcd.lcdWrite(lcd.centerText(output, 1, 14))
		
		
	def __nextFreq(self):
		previous = self.currentFreq
		self.currentFreq += 20
		if self.currentFreq > fmTx.maxFreq:
			self.currentFreq = fmTx.minFreq
			
		fmTx.setFrequency(self.currentFreq)
		self.__fmData()
		
		
	def __prevFreq(self):
		previous = self.currentFreq
		self.currentFreq -= 20
		if self.currentFreq < fmTx.minFreq:
			self.currentFreq = fmTx.maxFreq
			
		fmTx.setFrequency(self.currentFreq)
		self.__fmData()
			
			
	#processes button presses
	def __processBtn(self, button):
		if button != None:
			if button != self.prevButton:
				self.prevButton = button
		
				if button == 1:					#play/pause
					pass
				if button == 2:					#prev
					self.__prevFreq()
				if button == 3:					#next
					self.__nextFreq()

				if button == 4:					#mode/menu
					self.exitNow = True