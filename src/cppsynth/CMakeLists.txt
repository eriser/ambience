set( SOURCES
	 Env.cc
     Filter.cc
     Osc.cc
     Voice.cc
	 Synth.cc
	)
	 
set( HEADERS
	 Env.h
     Filter.h
     Osc.h
     Voice.h
	 Synth.h
	)

add_library(cppsynth
    ${SOURCES}
	${HEADERS}
    )
