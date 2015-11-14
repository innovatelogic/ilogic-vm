import os
import shutil

class ResDeploy:
	def __init__(self, source_dir, bin_dir):
		self.source_dir = source_dir
		self.bin_dir = bin_dir
		
	def deploy(self):
		self.copyBinResources(self.source_dir + "\\data\\resources", self.bin_dir + "\\resources")
		self.copyBinResources(self.source_dir + "\\data\\$cashe", self.bin_dir + "\\..\\data\\$cashe")
		self.copyBinResources(self.source_dir + "\\data\\$Games", self.bin_dir + "\\..\\data\\$Games")
		self.copyBinResources(self.source_dir + "\\data\\$registry", self.bin_dir + "\\..\\data\\$registry")
		self.copyBinResources(self.source_dir + "\\data\\$system", self.bin_dir + "\\..\\data\\$system")
		
	def copyBinResources(self, src, dest):
		try:
			shutil.copytree(src, dest)
    			# Directories are the same
		except OSError as e:
			print('Directory not copied. Error: %s' % e)