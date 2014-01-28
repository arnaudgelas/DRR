project( DRRFRomCT )

cmake_minimum_required( VERSION 2.8 )

find_package( ITK 4.5 REQUIRED )
include( ${ITK_USE_FILE} )

# find_package( Boost 1.54 REQUIRED COMPONENTS program_options )
# include_directories( ${Boost_INCLUDE_DIRS} )

add_executable( DRR DigitallyReconstructedRadiograph1.cxx )
target_link_libraries( DRR
  ${ITK_LIBRARIES}
#  ${Boost_LIBRARIES}
)


add_executable( DRR2 DRR2.cxx )
target_link_libraries( DRR2
  ${ITK_LIBRARIES}
#  ${Boost_LIBRARIES}
)
