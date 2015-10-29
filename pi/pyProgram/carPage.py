#!/usr/bin/python
#handles the car data page, displays various stats read from an obd2 scanner

#TODO: custom units (mph/kph, c/f etc)

import dispMod as lcd
import time
import threading
import obd
import subprocess

class Page(threading.Thread):
	
	subpage = ( [('INTAKE_TEMP', 'IAT', '') , ('RPM', '', ' RPM'),('INTAKE_PRESSURE', 'MAP', ' '), ('SPEED', '', 'KPH')],
		  [('MPG', '', 'MPG') , ('COOLANT_TEMP', 'TMP', 'C'),('COOLANT_TEMP', 'TMP', 'C'), ('COOLANT_TEMP', 'TMP', 'C')],
		  [('ENGINE_LOAD', 'LOAD', '%') , ('SPEED', '', ' MPH'),('MPG', '', ' MPG'), ('THROTTLE_POS', 'THR', '%')],
		  [('VOLTAGE', '', ' V') , ('COOLANT_TEMP', 'TMP', ' C'),('INTAKE_TEMP', 'IAT', 'C'), ('COOLANT_TEMP', 'TMP', 'C')] )
	
	subpageIndex = 1								#default and current subpage
	
	adaptorMAC = "AA:BB:CC:11:22:33"		#the mac address of the obd2 adapter
	adapterPort = "/dev/rfcomm1"
	
	btnPollRate = .05				#the rate at which the display module buttons are checked
	BtnMon = lcd.BtnChecker(btnPollRate)
	scrollSpeed = .15
	
	#car specific values needed for MPG calculation
	volumetricEfficiency = 85.0		#percentage
	engineDisplacement = 2.4 		#in liters
	mpgTrim = .91					#becuase I was still getting the wrong MPG
	
	#adaptorConnected = False
	adaptorConnected = True
	
	def __init__(self):
		threading.Thread.__init__(self)
		
		obd.debug.console = True
		
		self.prevButton = None				
		self.exitNow = False
		self.btportBound = False
		self.obdConnection = None
		self.previouslyConnected = False
		
		lcd.lcdClear()
		lcd.lcdSetCursor(0,0)
		lcd.lcdWrite("   Connecting   ")
		lcd.lcdSetCursor(0,1)
		lcd.lcdWrite("to OBD Reader...")
		
		#self.obdConnection = obd.OBD(self.adapterPort)		#start a new obd instance
		
		self.bindBtport()				#need to move this into a loop
		
		self.obdConnection = obd.Async(self.adapterPort)
		
		print("Car page ready to go")
		
		
		
		#runs the page, call this continuously for smooth operation
	def run(self):
		while(1):
			if not self.obdConnection.is_connected():
				if self.previouslyConnected:
					self.previouslyConnected = False
					
					lcd.lcdClear()
					lcd.lcdSetCursor(0,0)
					lcd.lcdWrite("  Reconnecting  ")
					lcd.lcdSetCursor(0,1)
					lcd.lcdWrite("to OBD Reader...")
			else:
				if self.previouslyConnected == False:
						self.previouslyConnected = True
						self.newSubpage()
					
				self.runSubpage()
		
			self.__processBtn(self.BtnMon.update())
			if self.exitNow:
				self.obdConnection.close()
				return


	#binds the bluetooth SSP deal to a rfcomm port thing, doing this also connects to the OBD2 adaptor (if it's paired)
	def bindBtport(self):
		command = 'rfcomm | grep ' + self.adaptorMAC				#command used to check the port status
		returncode = 0
	
		while(self.btportBound == False):							#while it's not yet bound	
			try:
				o = subprocess.check_output(command, shell=True)	#execute the above command
				print("rfcomm port already bound")					#we get here if the command was successful (exit 0)
				self.btportBound = True
			except subprocess.CalledProcessError as ex:				#if the command wasn't "successful" (exit whatever)
				o = ex.output
				returncode = ex.returncode							#get the return code
				if returncode != 1: 								#if it wasn't one, something else happened
					raise
					
				#assume the rfcomm port is not bound yet, so do that now
				command = "rfcomm bind " + self.adapterPort + " " + self.adaptorMAC + " 1"
				print(subprocess.check_output(command, shell=True))		#run the above command, print the result
	
	
	def runSubpage(self):
		#timer in here to reduce refresh rate
		self.refreshSubpage()
		time.sleep(.1)
		
		
	#gets new data values from the car, and displays them
	def refreshSubpage(self):
		currentSubpage = self.subpage[self.subpageIndex]
	
		self.printData(currentSubpage[0], False, 0)
		self.printData(currentSubpage[1], False, 1)
		self.printData(currentSubpage[2], True, 0)
		self.printData(currentSubpage[3], True, 1)
		

		
	#wants a tuple
	def printData(self, commandTuple, rightAlign, y):
		value = None
		command = commandTuple[0]
		name = commandTuple[1]
		unit = commandTuple[2]
		
		if command == "MPG":
			value = self.calcMPG()
		else:
			cmd = obd.commands[command]
			response = self.obdConnection.query(cmd) # send the command, get the response
			value = response.value
			
		if value == None:
			value = '--'
			
		if name != "":
			name = name + " "
		
		output = name + str(value) + unit
			
		if rightAlign:
			lcd.lcdSetCursor(16 - len(output),y)
			print(output)
			lcd.lcdWrite(output)
		else:
			lcd.lcdSetCursor(0,y)
			print(output)
			lcd.lcdWrite(output)
			
			
	#calculates the current MPG, no running averages here
	def calcMPG(self):
		#current RPM
		rpm = self.obdConnection.query(obd.commands.RPM)
		rpm = rpm.value
		
		#manifold absolute pressure (my car doesn't have a MAF) in kpa
		map = self.obdConnection.query(obd.commands.INTAKE_PRESSURE)
		map = map.value
		
		#intake air temp, converted from celcius to kelvin
		iat = self.obdConnection.query(obd.commands.INTAKE_TEMP)
		iat = iat.value
		
		#current speed in kph
		speed = self.obdConnection.query(obd.commands.SPEED)
		speed = speed.value
		
		mpg = 0
		
		if rpm != None and map != None and iat != None and speed != None:
			#calculate the instantaneous manifold absolute pressure (????)
			imap = rpm * map / (iat + 273)
			
			#calcuate the "fake" maf
			maf = (imap/120)*(self.volumetricEfficiency/100)*(self.engineDisplacement)*(28.97)/(8.314)
			
			if maf != 0:
				mpg = 7.107335 * speed / maf * self.mpgTrim
			
		return "{0:.1f}".format(mpg)
			
			
	def nextDataPage(self):
		self.subpageIndex += 1
		if self.subpageIndex >= len(self.subpage):
			self.subpageIndex = 0
		self.newSubpage()
	
	
	def prevDataPage(self):
		self.subpageIndex -= 1
		if self.subpageIndex < 0:
			self.subpageIndex = len(self.subpage) - 1
		self.newSubpage()

		
		
	
	def newSubpage(self):
		currentSubpage = self.subpage[self.subpageIndex]

		self.obdConnection.stop() 				#stop the async
		self.obdConnection.unwatch_all()		#unwatch all previous 
		
		for sensor in currentSubpage: 
			#print(str(sensor[0]))
			
			if sensor[0] == "MPG":				#if there is a MPG section in this subpage
			
				#watch these pids too since they are needed for the MPG calculation
				mpgCmds = ["RPM", "INTAKE_PRESSURE", "INTAKE_TEMP", "SPEED"]
				
				for cmd in mpgCmds:
					command = obd.commands[cmd]
					self.obdConnection.watch(command, callback=None, force=True)

			else:
				command = obd.commands[sensor[0]]
				self.obdConnection.watch(command, callback=None, force=True)	
		
		self.obdConnection.start()				#start the async
		
		lcd.lcdClear()
		
	
	#processes button presses
	def __processBtn(self, button):
		if button != None:
			if button != self.prevButton:
				self.prevButton = button
		
				if self.adaptorConnected:
					if button == 2:					#prev
						self.prevDataPage()
					if button == 3:					#next
						self.nextDataPage()
				if button == 4:					#mode/menu
					self.exitNow = True