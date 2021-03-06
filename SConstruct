nameProgram = 'province'

agents = ['Roman']
world = 'Province'
namespaceAgents = ['Epnet']

srcFiles = Split('main.cxx Roman.cxx Province.cxx ProvinceConfig.cxx HarvestAction.cxx ProposeConnectionAction.cxx SendGoodsAction.cxx FunAction.cxx ProposeTradeAction.cxx RandomController.cxx BasicController.cxx ControllerFactory.cxx MacmillanController.cxx ProductionAction.cxx TradeAction.cxx ConsumptionAction.cxx CulturalAction.cxx Network.cxx')


import os, sys
from subprocess import call

pandoraPath = os.getenv('PANDORAPATH')

sys.path.append(pandoraPath+'/bin')

import generateMpi 

release = ARGUMENTS.get('release', 1)
extremeDebug = ARGUMENTS.get('edebug', 0)
useMpi = ARGUMENTS.get('mpi', 1)

if int(useMpi) == 1:
	env = Environment(ENV=os.environ, CXX='mpicxx')
else:
	env = Environment(ENV=os.environ, CXX='g++')

generateMPICodeBuilder = Builder(action=generateMpi.execute)
env.Append( BUILDERS = {'GenerateMPICode' : generateMPICodeBuilder})



linkFlags = Split('-fopenmp')
libs = Split('tinyxml pthread grass_datetime grass_gis  boost_filesystem boost_system hdf5 gdal boost_timer boost_chrono')
if int(release) == 0:
	env['CCFLAGS'] = Split('-g -O0 -Wall -std=c++0x -DTIXML_USE_STL -fopenmp -DPANDORADEBUG -DMPICH_IGNORE_CXX_SEEK')
	if int(extremeDebug)==1:
		env['CCFLAGS'] += ['-DPANDORAEDEBUG']
	libs += ['pandorad']
else:
	env['CCFLAGS'] = Split('-O3 -DTIXML_USE_STL -std=c++0x -fopenmp -DMPICH_IGNORE_CXX_SEEK')
	libs += ['pandora']

if int(useMpi)==1:
	env['CCFLAGS'] += ['-DPANDORAMPI-std=c++0x ']

includeDirs = Split('./ /home/bsc21/bsc21394/pandora/include /home/bsc21/bsc21394/libs/libmdp')
includeDirs += ['/apps/HDF5/1.8.13-mpi/OPENMPI/GCC/include']
includeDirs += ['/apps/BOOST/1.56.0/OPENMPI/include']
includeDirs += ['/apps/TINYXML/2.6.2/include']
includeDirs += ['/apps/GDAL/1.9.2/include']
includeDirs += ['/apps/GRASS/6.4.3RC2/grass-6.4.3RC2/include/']

libDirs = Split('/home/bsc21/bsc21394/pandora/lib /apps/HDF5/1.8.13-mpi/OPENMPI/GCC/lib /apps/GDAL/1.9.2/lib /apps/TINYXML/2.6.2/lib')
libDirs += ['/apps/BOOST/1.56.0/OPENMPI/lib/']
libDirs += ['/apps/GRASS/6.4.3RC2/grass-6.4.3RC2/lib/']
env.Append( RPATH = '/apps/GRASS/6.4.3RC2/grass-6.4.3RC2/lib/')

# add the list of mpi code that must be generated & compiled
mpiAgentsSrc = ['mpiCode/FactoryCode.cxx']
agentsSrc = ['main.cxx']
for agent in agents:	
	if agent != '':
		agentsSrc.append(agent+".cxx")
		mpiAgentsSrc.append("mpiCode/"+agent+"_mpi.cxx")

env['namespaces'] = namespaceAgents
env.GenerateMPICode( target=mpiAgentsSrc, source=agentsSrc)
env.Depends(world+'.hxx',mpiAgentsSrc)
env.Program(nameProgram, srcFiles+mpiAgentsSrc, CPPPATH=includeDirs, LIBS=libs, LIBPATH=libDirs, LINKFLAGS=linkFlags)

