import os
import shutil

generators = {
    'vc110' : ('Visual Studio 11 2012', 'v110_xp'),
    'vc120' : ('Visual Studio 12 2013', None)
    }

def checkPythonVer(in_ver):
	py_ver = sys.version_info[:2]
	if py_ver != in_ver:
		raise Exception("Incompatible python version: need %s", str(py_ver))

class Config:
	cmake_exe = None
	cmake_modules = None
	cur_file_dir = os.path.dirname(__file__)
	#source_dir = os.path.dirname(cur_file_dir)
	#cmake_exe = os.path.join(source_dir, "utils\\cmake-3.2.1\\bin\\cmake.exe")
	#root_dir = os.path.dirname(source_dir)

	def __init__(self, name, gen_id, amd64, xpSupport, source_dir, out_dir, bin_dir, install=False):
		self.name = name
		self.gen_id = gen_id
		self.amd64 = amd64
		out = 'out64' if amd64 else 'out32'
		
		self.generator = generators[gen_id][0]
		self.generatorToolset = generators[gen_id][1] if xpSupport else None
		
		self.source_dir = source_dir
		#self.install_dir = os.path.join(self.root_dir, out)
		self.out_dir = out_dir #os.path.join(self.root_dir, out +'\\build_' + target + '_' + genID)
		self.bin_dir = bin_dir #os.path.join(self.root_dir, bindir) if bindir else os.path.join(self.install_dir, 'bin')
		
		self.install = install
  
	def clearWorkingDir(self):
		if os.path.exists(self.out_dir):
		  shutil.rmtree(self.out_dir)
		pass
		
	def clearBinDir(self):
		if os.path.exists(self.bin_dir):
			shutil.rmtree(self.bin_dir + '\..')
		pass
		
	def ensure_dir(f):
		d = os.path.dirname(f)
		if not os.path.exists(d):
			os.makedirs(d)
    
	def generate(self):
		print("generate...")
		print('source_dir:' + self.source_dir)
		print('cmake_bin:' + self.cmake_exe) #cmake_dir
		print('target:' + self.name)
		print('generator:' + self.generator + (' x64' if self.amd64 else ' x86'))
		#print('install_dir:' + self.install_dir)
		print('out_dir:' + self.out_dir)
		print('bin_dir:' + self.bin_dir)
		
		self.clearBinDir()
		self.clearWorkingDir()
		
		cmd_gen = self.cmake_exe
		cmd_gen += ' -G"%s"' % self.generator
		cmd_gen += ' -H"%s"' % self.source_dir
		cmd_gen += ' -B"%s"' % self.out_dir
		cmd_gen += ' -DG_GENERATOR_TOKEN="%s"' % self.gen_id
		cmd_gen += ' -DG_CMAKE_TARGET="%s"' % self.name
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
    
		if not os.path.exists(self.out_dir):
			os.makedirs(self.out_dir)

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
		cmd_build = r'%s --build %s' % (self.cmake_exe, self.out_dir)
		
		if config:
			cmd_build += r' --config %s' % config
		
		#i = install if install is not None else self.install
		#if i:
		#	cmd_build += r' --target INSTALL'
		  
		print(cmd_build)
		res = os.system(cmd_build)
		if res != 0:
			raise Exception
		print('build_finish:' + config)

	def copyBinResources(self, src, dest):
		try:
			shutil.copytree(src, dest)
    			# Directories are the same
		except OSError as e:
			print('Directory not copied. Error: %s' % e)
		