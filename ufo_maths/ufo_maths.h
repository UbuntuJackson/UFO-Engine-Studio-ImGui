#ifndef UFO_MATHS_H
#define UFO_MATHS_H
#include <cmath>
#include <cassert>

namespace ufoMaths{

template<typename tNumber>
tNumber Wrap(tNumber _value, tNumber _min, tNumber _max){
    assert(_min < _max);
    tNumber value_relative_to_min = _value - _min;
    tNumber range = _max - _min;
    if(value_relative_to_min > 0){
        return _min + std::fmod(value_relative_to_min, range);
    }
    if(value_relative_to_min < 0){
        return _max + std::fmod(value_relative_to_min, range);
    }
    return _value;
}

int Wrapi(int _value, int _min, int _max);

}

#endif