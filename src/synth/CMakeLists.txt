set( SOURCES
	 env.c
     filter.c
     osc.c
     synth.c
     SynthWrapper.cc
     voice.c
	)
	 
set( HEADERS
	 env.h
     filter.h
     osc.h
     synth.h
     SynthWrapper.h
     voice.h
	)

add_library(synth
    ${SOURCES}
	${HEADERS}
    )
