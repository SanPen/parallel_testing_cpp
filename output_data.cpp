//
// Created by santi on 26/08/2022.
//

#include <cassert>
#include "output_data.h"

OutputData::OutputData(int time_steps, int dim) :
        _dim(dim),
        _time_steps(time_steps),
        _mat(dim, time_steps)
{}

Matrix OutputData::setAt(int t, const Matrix& m) {
    assert(m.getRows() == _dim);

    for (int i=0; i < _dim ; ++i)
        _mat(i, t) = m(i, 0);

    return m;
}

int OutputData::getDim() const {
    return _dim;
}

int OutputData::getTimeSteps() const {
    return _time_steps;
}

const Matrix &OutputData::getMat() const {
    return _mat;
}
