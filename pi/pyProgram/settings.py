#!/usr/bin/python
#handles save data reading and writing

import pickle
import os.path

filename = None

#default variable settings
saveData = { 'brightness': 10, "contrast": 30, "frequency": 8790, "textSpeed": 10 }

def init(inFile):
	global saveData
	global filename
	filename = inFile
	if not os.path.isfile(filename):	#if the freqFile does not exist
		#create an empty file, and write the default values to it
		pickle.dump(saveData, open(filename, 'w'))
		saveData = pickle.load( open(filename, "rb"))
		
def save():
	pickle.dump(saveData, open(filename, 'w'))