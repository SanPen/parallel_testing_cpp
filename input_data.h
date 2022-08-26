//
// Created by santi on 26/08/2022.
//

#ifndef NEWTON_INPUT_DATA_H
#define NEWTON_INPUT_DATA_H

#include "matrix.h"

class InputData {

    int _dim;
    int _time_steps;
    Matrix _mat;

public:

    InputData(int time_steps, int dim);

    [[nodiscard]] Matrix getAt(int t) const;

    int getDim() const;

    int getTimeSteps() const;

    const Matrix &getMat() const;

};


#endif //NEWTON_INPUT_DATA_H
