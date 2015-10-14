#!/usr/bin/python
#handles general "page" control
#pages are like different display modes (music, car data, settings, etc)

import dispMod as lcd
import time
import threading
import musicPage as music
import fmPage as fm
import carPage as car
import btPage as bt
import settingPage as settings

currentPage = None
currentPageThread = None
pagerator = None
pagerating = False

pageIndex = 2
lastPage = 2

pageVars = {0: music, 1: fm, 2: car, 3: bt, 4: settings}
pageNames = {0: "Music", 1: "FM Control", 2: "Car Data", 3: "Bluetooth", 4: "Settings"}


#gets all the pages ready to go and displays the boot screen
def init():
	startupAnim()
	setPage(pageIndex)

	
#displays the currently selected page, call this continuously
def main():
	while(1):
		global currentPage
		if not currentPage.isAlive():			#if the current page ended
			global pagerating
			if not pagerating:
				global pageIndex
				pagerating = True
				pagerator = Pagerator(pageIndex)
				pagerator.start()
			if not pagerator.isAlive():
				pagerating = False
				setPage(pageIndex)

def setPage(newPage):
	global currentPage
	print("got: " + str(newPage))
	currentPage = pageVars[newPage].Page()
	currentPage.start()

		
#shows the startup animation on the display
def startupAnim():
	lcd.lcdSetCursor(1, 0)
	lcd.lcdWrite(0)
	lcd.lcdWrite(1)
	time.sleep(.25)

	lcd.lcdSetCursor(0, 1)
	lcd.lcdWrite(3)
	lcd.lcdWrite(2)
	time.sleep(.25)

	lcd.lcdWrite(4)
	lcd.lcdWrite(5)
	time.sleep(.25)

	lcd.lcdSetCursor(6, 0)
	lcd.lcdWrite("carbt_v1")
	lcd.lcdSetCursor(6, 1)
	lcd.lcdWrite("Starting")
	time.sleep(1)
	

#a special "page", it handles switching to new pages
class Pagerator(threading.Thread):

	def __init__(self, lastPage):
		threading.Thread.__init__(self)
		
		self.lastPage = lastPage
		self.pages = len(pageNames)
		self.currentSelection = self.lastPage

		lcd.lcdClear()
		lcd.lcdSetCursor(0,0)
		lcd.lcdWrite(" Select a Page ")
		lcd.lcdSetCursor(0,1)
		lcd.lcdWrite(251)
		lcd.lcdSetCursor(1,1)
		lcd.lcdWrite(lcd.centerText(pageNames[self.lastPage], 1, 14))
		lcd.lcdSetCursor(15,1)
		lcd.lcdWrite(252)
		
		self.done = False
		self.btnPollRate = .1
		self.prevButton = None
		self.BtnMon = lcd.BtnChecker(self.btnPollRate)
		self.scrolling = False
		self.Slider = lcd.LcdScrollOneshot(pageNames[self.lastPage], .05, 1, 14, 1, True)
		
		print("pagerator is now pagerating")
			
		
	def run(self):
		while(1):
			self.__processBtn(self.BtnMon.update())
			self.__showSelection()
			
			if self.done:
				global pageIndex
				pageIndex = self.currentSelection
				return
		
	def __showSelection(self):
		if self.scrolling:
			if self.Slider.scroll() == False:
				self.scrolling = False
		
		
	def __nextPage(self):
		previous = self.currentSelection
		self.currentSelection += 1
		if self.currentSelection == self.pages:
			self.currentSelection = 0
		self.Slider.updateOutput(pageNames[previous], pageNames[self.currentSelection], True)
		self.scrolling = True
	
	
	def __prevPage(self):
		previous = self.currentSelection
		self.currentSelection -= 1
		if self.currentSelection < 0:
			self.currentSelection = self.pages - 1
		self.Slider.updateOutput(pageNames[previous], pageNames[self.currentSelection], False)
		self.scrolling = True
	
	
	def __processBtn(self, button):
		if button != None:						#if we got a response from the display module
			if button != self.prevButton:		#if it's a unique/new button
				self.prevButton = button
		
				if button == 1:					#play/pause/select/OK
					#if the button was pressed, we need to start the selected channel
					self.done = True
				if button == 2:					#prev
					#select the previous page in the list
					self.__prevPage()
				if button == 3:					#next
					#select the next page in the list
					self.__nextPage()