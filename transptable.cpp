#include "transptable.h"
#include "targetsearch.h"
#include "inout.h"

TERM::TERM(){
	insert<SHAPETERM>("\
				......	\
				......	\
				......	", 
		   BIND<SHAPETERM, int>()( 2, DAFEI_TM, TIAO_TM) ); 
	insert<SHAPETERM>("\
				......	\
				......	\
				......	", 
		   BIND<SHAPETERM, int>()( 2, DAFEI_TM, TIAO_TM) ); 
	insert<  LIBTERM>("\
				......	\
				......	\
				......	", 
		   BIND<  LIBTERM, int>()( 2, ZIJINQI_TM, TI_TM) ); 

}

