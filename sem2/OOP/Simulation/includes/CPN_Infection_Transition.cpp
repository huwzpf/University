#include "CPN_Infection_Transition.h"

void CPN_Infection_Transition::calc(double dt) {
	d_in = - (parameter * *out_connection * *in_connection * dt);
	d_out = -d_in;
}