#include <cmath>
#include <math.h>
#include "ufo_maths.h"

namespace ufoMaths{

int Wrapi(int _value, int _min, int _max){
    assert(_min < _max);
    if(_value < 0) _value = std::abs(_max+_value%(_max-_min));
    
    int range = _max - _min; //1
    
    return _min + _value % range;
    
}

}