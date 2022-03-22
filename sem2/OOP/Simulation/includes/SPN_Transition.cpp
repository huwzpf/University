#include "SPN_Transition.h"
void SPN_Transition::shift() {
	if (*in_connection > 0) {
		(*in_connection) -= 1;
		(*out_connection) += 1;
	}
	
}

void SPN_Transition::update() {
	update_lambda();
	shift();
}