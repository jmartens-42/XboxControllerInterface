#include "utilities.h"



float map(float in, float in_low, float in_high, float out_low, float out_high){

    return (((in - in_low) * ((out_high - out_low) / (in_high - in_low))) + out_low);


}