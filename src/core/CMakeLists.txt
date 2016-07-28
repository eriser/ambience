set( SOURCES
     Chromosome.cc
     Individual.cc
     Population.cc
     Representation.cc
	)
	 
set( HEADERS
     Representation.h
     Chromosome.h
     Individual.h
     Population.h
     Evaluator.h
	)

add_library( core
    ${SOURCES}
	${HEADERS}
    )
