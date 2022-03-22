#include "SPN_Positive_Transition.h"


void SPN_Positive_Transition::update_lambda() {
	lambda = *in_connection * parameter;
}
