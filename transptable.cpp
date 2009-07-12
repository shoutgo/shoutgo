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

/* _______________________________ ²âÊÔÇø _______________________________ */

void TEST_TT::aimtree(){
	AIMGRAPH aimtree; 
	for (int i = 0; i<AIM_SIZE; ++i){
		AIM a = static_cast<AIM>(i); 
		___COUT1(aimtree.sons [a].size () ); 
		___COUT3(aimtree.backward(a), a, aimtree.forward(a)); 
		___ASSERT(aimtree.anti(aimtree.anti (a)) == a ); 
	}
}

void TEST_TT::timefunc (string which) {
	RANDER  r; 
}
