import os
import sys
import shutil
import argparse

def checkPythonVer(in_ver):
	py_ver = sys.version_info[:2]
	if py_ver != in_ver:
		raise Exception("Incompatible python version: need %s", str(py_ver))
    
generators = {
    'vc110' : ('Visual Studio 11 2012', 'v110_xp'),
    'vc120' : ('Visual Studio 12 2013', None)
    }

class Config:
	cur_file_dir = os.path.dirname(__file__)
	source_dir = os.path.dirname(cur_file_dir)
	cmake_exe = os.path.join(source_dir, "utils\\cmake-3.2.1\\bin\\cmake.exe")
	root_dir = os.path.dirname(source_dir)

	def __init__(self, target, genID, amd64, xpSupport, install, bindir=None):
		self.genID = genID
		self.target = target
		self.amd64 = amd64
		out = 'out64' if amd64 else 'out32'
		
		self.generator = generators[genID][0]
		self.generatorToolset = generators[genID][1] if xpSupport else None
		
		self.install_dir = os.path.join(self.root_dir, out)
		self.working_dir = os.path.join(self.root_dir, out +'\\build_' + target + '_' + genID)
		self.bin_dir = os.path.join(self.root_dir, bindir) if bindir else os.path.join(self.install_dir, 'bin')
		
		self.install = install
  
	def clearWorkingDir(self):
		if os.path.exists(self.working_dir):
		  shutil.rmtree(self.working_dir)
		pass
		
	def ensure_dir(f):
		d = os.path.dirname(f)
		if not os.path.exists(d):
			os.makedirs(d)
    
	def generate(self):
		print('source_dir:' + self.source_dir)
		print('cmake_bin:' + self.cmake_exe) #cmake_dir
		print('target:' + self.target)
		print('generator:' + self.generator + (' x64' if self.amd64 else ' x86'))
		print('install_dir:' + self.install_dir)
		print('working_dir:' + self.working_dir)
		print('bin_dir:' + self.bin_dir)
		
		self.clearWorkingDir()
		
		cmd_gen = self.cmake_exe
		cmd_gen += ' -G"%s"' % self.generator
		cmd_gen += ' -H"%s"' % self.source_dir
		cmd_gen += ' -B"%s"' % self.working_dir
		cmd_gen += ' -DG_GENERATOR_TOKEN="%s"' % self.genID
		cmd_gen += ' -DG_CMAKE_TARGET="%s"' % self.target
		cmd_gen += ' -DCMAKE_INSTALL_PREFIX="%s"' % self.bin_dir
		cmd_gen += ' -DG_BIN_DIR="%s"' % self.bin_dir
		#cmd_gen = r'%s -DG_GENERATOR_TOKEN="%s" -DG_CMAKE_TARGET="%s" "%s"' %(self.cmake_exe, self.genID, self.target, self.source_dir)
		
		#cmd_gen = r'%s -G "%s" -B"%s" -REMOVE "%s" -DCMAKE_INSTALL_PREFIX="%s" -DG_CMAKE_TARGET="%s"' %(self.cmake_exe, self.source_dir, self.generator, self.source_dir, #self.working_dir, self.install_dir, self.target)
		
		#if self.bin_dir != "":
		#	cmd_gen += r' _D_BIN_DIR="%s"' % self.bin_dir
		  
		if self.amd64:
			cmd_gen += r' -DG_PROCESSOR_ARCH=x64'
		else:
			cmd_gen += r' -DG_PROCESSOR_ARCH=x86'
			
		print(cmd_gen)
    
		if not os.path.exists(self.working_dir):
			os.makedirs(self.working_dir)

		res = os.system(cmd_gen)
    
		if res != 0:
		  raise Exception

		#install resources
		self.copyBinResources(self.source_dir + "\\bin\\resources", self.bin_dir + "\\resources")
		self.copyBinResources(self.source_dir + "\\$cashe", self.bin_dir + "\\..\\$cashe")
		self.copyBinResources(self.source_dir + "\\$Games", self.bin_dir + "\\..\\$Games")
		self.copyBinResources(self.source_dir + "\\$registry", self.bin_dir + "\\..\\$registry")
		self.copyBinResources(self.source_dir + "\\$system", self.bin_dir + "\\..\\$system")
    
	def build(self, config = None):
		cmd_build = r'%s --build %s' % (self.cmake_exe, self.working_dir)
		
		if config:
			cmd_build += r' --config %s' % config
		if self.install:
			cmd_build += r' --target INSTALL'
		  
		print(cmd_build)
		res = os.system(cmd_build)
		if res != 0:
			raise Exception

	def copyBinResources(self, src, dest):
		try:
			shutil.copytree(src, dest)
    			# Directories are the same
		except OSError as e:
			print('Directory not copied. Error: %s' % e)
		

def main():
	checkPythonVer((3,4))
	
	parser = argparse.ArgumentParser()
	parser.add_argument('--goal', type=str, help='')
	args = parser.parse_args()

	client_deps = Config('client_dep', 'vc110', amd64=False, xpSupport=True, install=True, bindir='bin\\client')
	client =      Config('client', 'vc120', amd64=False, xpSupport=True, install=False, bindir='bin\\client')
  
	try:
		if args.goal == None:
			print('None args')
		elif args.goal == 'client_release':
			client_deps.generate()
			#client_deps.buld('Release')
		elif args.goal == 'client_debug':
			client_deps.generate()
			#clinet_deps.build('Debug')
	except Exception:
		print(sys.exc_info()[0])
		print(sys.exc_info()[1])
		
	return 1
	
if __name__ == "__main__": 
	main()	
	