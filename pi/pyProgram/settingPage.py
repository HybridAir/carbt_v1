#!/usr/bin/python
#handles device settings control

#	Select an option
#	<   settings   >

import dispMod as lcd
import time
import threading

class Page(threading.Thread):

	def __init__(self):
		threading.Thread.__init__(self)
	
		output = "this is a super long string of text that the device module is going to scroll lol"
		print("init settingpage")	

	#runs the page, call this continuously for smooth operation
	def run(self):
		time.sleep(5)
		print("returning")
		return