//
// Created by santi on 26/08/2022.
//

#include "input_data.h"

InputData::InputData(int time_steps, int dim) :
        _dim(dim),
        _time_steps(time_steps),
        _mat(dim, time_steps)
{}

Matrix InputData::getAt(int t) const {
    Matrix m(_dim, 1);
    for (int i=0; i < _dim ; ++i)
        m(i, 0) = _mat(i, t);
    return m;
}

int InputData::getDim() const {
    return _dim;
}

int InputData::getTimeSteps() const {
    return _time_steps;
}

const Matrix &InputData::getMat() const {
    return _mat;
}
