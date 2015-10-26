import os, sys, shutil, argparse
from functools import partial
from scripts import cmake
from scripts import res_dep
import subprocess
import traceback

def checkPythonVer(in_ver):
	py_ver = sys.version_info[:2]
	if py_ver != in_ver:
		raise Exception("Incompatible python version: need %s", str(py_ver))
		
def clearDir(dir):
	if os.path.exists(dir):
		shutil.rmtree(dir)
	pass
		
def main():
	checkPythonVer((3,4))
	
	parser = argparse.ArgumentParser()
	parser.add_argument('--goal', type=str, help='')
	args = parser.parse_args()
	
	cur_file_dir = os.path.dirname(os.path.realpath(__file__))
	source_dir = os.path.dirname(cur_file_dir)
	root_dir = root_dir = os.path.dirname(source_dir)
	cmake_dir = os.path.join(source_dir, "utils/cmake-3.2.1/bin")
	
	#clearDir(os.path.join(root_dir, 'out32'))

	cmake.Config.cmake_exe = os.path.join(cmake_dir, 'cmake.exe')
	#client_deps = cmake.Config('client_dep', gen_id='vc110', amd64=False, xpSupport=True, install=True, source_dir=source_dir, out_dir=os.path.join(root_dir, 'out32'), #bin_dir=os.path.join(root_dir, 'out32/bin'))
	#client =      cmake.Config('client', 'vc120', amd64=False, xpSupport=True, install=False, bindir='bin\\client')

	#
	#cur_file_dir = os.path.dirname(os.path.realpath(__file__))
	#engine_source_dir = os.path.dirname(cur_file_dir)
	#root_dir = os.path.dirname(programming_dir)
	
	#print(cur_file_dir)
	#print(programming_dir)
	#print(root_dir)

	specs = dict()
	
	specs['tools_dep'] = {
		'gen_id': 'vc110',
		'amd64': False,
		'xpSupport': True,
		'source_dir': source_dir,
		'out_dir': os.path.join(root_dir, 'out32'),
		'bin_dir': os.path.join(root_dir, 'out32/bin'),
		#'args': {
		#},
		'install': True
	}
	
	specs['tools'] = {
		'gen_id': 'vc110',
		'amd64': False,
		'xpSupport': True,
		'source_dir': source_dir,
		'out_dir': os.path.join(root_dir, 'out32'),
		'bin_dir': os.path.join(root_dir, 'out32/bin'),
		#'args': {
		#},
		'install': True
	}
	
	tools_deps = cmake.Config('tools_dep', **specs['tools_dep'])
	tools = cmake.Config('tools', **specs['tools'])
	
	res_deploy = res_dep.ResDeploy(source_dir, os.path.join(root_dir, 'out32/bin'))
	
	try:
		if args.goal == None:
			print('None args')
		elif args.goal == 'tools_build':
			tools_deps.generate()
			
			tools_deps.build('Debug')
			#tools_deps.build('Release')
			tools.generate()
			tools.build('Debug')
			#tools.build('Release')
			
		res_deploy.deploy()
		
	except Exception:
		print(sys.exc_info()[0])
		print(sys.exc_info()[1])
		
	return 1
	
if __name__ == "__main__": 
	main()	
	