#!/usr/bin/python
#primary handler for everything
#bluetooth ops, song data, song playback control, display module ops, all that fun stuff
#this needs to always be running in the background, and be ran as root

import traceback
import sys

import time
import threading
import bluePlayer as bt

import dispMod
import fmTx
import settings
import pager

fileName = "saveData"


def main():
	print("Main program starting")
	
	#run the bootup animation first in a new thread
	#while that goes, run the rest of the init atuff
	#don't continue until both are done
	
	settings.init(fileName)
	dispMod.init()
	fmTx.init()
	pager.init()
	
	

	
	


	try:
		pagerThread = threading.Thread(target=pager.main)
		btThread = bt.BluePlayer()
		pagerThread.start()
		btThread.start()
		
		
	except Exception, err:
		print(traceback.format_exc())

if __name__ == "__main__":
	main()
