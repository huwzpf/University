#include "CPN_Positive_Transition.h"


void CPN_Positive_Transition::calc(double dt) {
	 d_in = -(parameter * *in_connection * dt);
	 d_out = -d_in;
}
