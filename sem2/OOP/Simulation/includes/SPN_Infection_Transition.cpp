#include "SPN_Infection_Transition.h"

void SPN_Infection_Transition::update_lambda() {
	lambda = *in_connection * *out_connection * parameter;
}


