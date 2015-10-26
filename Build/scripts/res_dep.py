import os
import shutil

class ResDeploy:
	def __init__(self, source_dir, bin_dir):
		self.source_dir = source_dir
		self.bin_dir = bin_dir
		
	def deploy(self):
		self.copyBinResources(self.source_dir + "\\bin\\resources", self.bin_dir + "\\resources")
		self.copyBinResources(self.source_dir + "\\$cashe", self.bin_dir + "\\..\\$cashe")
		self.copyBinResources(self.source_dir + "\\$Games", self.bin_dir + "\\..\\$Games")
		self.copyBinResources(self.source_dir + "\\$registry", self.bin_dir + "\\..\\$registry")
		self.copyBinResources(self.source_dir + "\\$system", self.bin_dir + "\\..\\$system")
		
	def copyBinResources(self, src, dest):
		try:
			shutil.copytree(src, dest)
    			# Directories are the same
		except OSError as e:
			print('Directory not copied. Error: %s' % e)