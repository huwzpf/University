#include "CPN_Transition.h"
void CPN_Transition::update() {

	*in_connection += d_in;
	*out_connection += d_out;

}