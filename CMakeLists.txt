# If you want to create your own example
# Create a CMakeLists.txt containing something similar to this
# You should edit:
#   * ceeculture
#   * your_EXEC_name
#   * ceec
#   * your_agent_name
#   * all_your_files.cxx <= Add all the sources that you need

cmake_minimum_required (VERSION 3.1)
project( ceeculture )

find_package( OpenMP )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )

find_package(MPI)
include_directories( ${MPI_INCLUDE_PATH} )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DGDAL_VERSION_1 -DTIXML_USE_STL" )

set( EX_NAME ceeculture )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} $ENV{PANDORAPATH}/include )
link_directories( $ENV{PANDORAPATH}/lib )

set( NAMESPACE Epnet )
set( FILE_NAME Roman )
add_custom_command(
    OUTPUT   mpiCode/FactoryCode.cxx mpiCode/${FILE_NAME}_mpi.cxx
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx .
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/${FILE_NAME}.* .
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Roman.hxx .
    COMMAND PYTHONPATH=${CMAKE_SOURCE_DIR}/bin ${CMAKE_SOURCE_DIR}/bin/mpiCode.py ${NAMESPACE} ${FILE_NAME} 
    DEPENDS main.cxx ${FILE_NAME}.cxx ${FILE_NAME}.hxx
)

# add the executable
add_executable( ${EX_NAME}
                BasicController.cxx
                ConsumptionAction.cxx
                ControllerFactory.cxx
                CulturalAction.cxx
                FunAction.cxx
                HarvestAction.cxx
                MacmillanController.cxx
                main.cxx
                Network.cxx
                ProductionAction.cxx
                ProposeConnectionAction.cxx
                ProposeTradeAction.cxx
                ProvinceConfig.cxx
                Province.cxx
                RandomController.cxx
                Roman.cxx
                SendGoodsAction.cxx
                TradeAction.cxx
                #AnalyseTools/NoState.cxx
                mpiCode/FactoryCode.cxx
                mpiCode/${FILE_NAME}_mpi.cxx
                )
target_link_libraries( ${EX_NAME}
                       pandora 
                       ${MPI_CXX_LIBRARIES} )

