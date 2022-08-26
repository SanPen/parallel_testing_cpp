//
// Created by santi on 26/08/2022.
//

#ifndef NEWTON_OUTPUT_DATA_H
#define NEWTON_OUTPUT_DATA_H


#include "matrix.h"

class OutputData {

    int _dim;
    int _time_steps;
    Matrix _mat;

public:

    OutputData(int time_steps, int dim);

    Matrix setAt(int t, const Matrix& m);

    int getDim() const;

    int getTimeSteps() const;

    const Matrix &getMat() const;

};


#endif //NEWTON_OUTPUT_DATA_H
