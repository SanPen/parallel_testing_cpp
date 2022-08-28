//
// Created by santi on 26/08/2022.
//
//#include <ppl.h>
#include "input_data.h"
#include "output_data.h"
#include "random.h"
#include "par_for.h"

void fillA(Matrix& A){

    Random rnd;
    rnd.setTimeBasedSeed();

    for(int i=0; i < A.getRows(); ++i)
        for(int j=0; j < A.getRows(); ++j)
            A(i, j) = (int) rnd.randInt(0, 1000);

}


void worker(const InputData& input_data,
            OutputData& output_data,
            const std::vector<int>& time_indices,
            int thread_index){

    std::cout << "Thread " << thread_index << " [" << time_indices[0]<< ", " << time_indices[time_indices.size() - 1] << "]\n";

    Matrix A(input_data.getDim(), input_data.getDim());
    Matrix b(input_data.getDim(), 1);
    Matrix x(input_data.getDim(), 1);

    for(const int& t: time_indices){

        b = input_data.getAt(t);

        fillA(A);

        x = A * b;

        output_data.setAt(t, x);
    }

}


void process(int time_steps, int dim, int n_threads){
    InputData input_data(time_steps, dim);
    OutputData output_data(time_steps, dim);

    // correct the number of threads
    if ( n_threads < 1 ) { n_threads = ( int )getThreadNumber( ); }

    // generate indices
    std::vector<int> time_indices = arrange<int>(time_steps);

    // compute the split of indices per core
    std::vector<ParallelChunkData<int>> chunks = prepareParallelChunks(time_indices, n_threads );

    // run in parallel
    ParallelFor( 0, ( int )chunks.size( ), [ & ]( int k ) {
            // run chunk
            worker(input_data, output_data, chunks[k].indices, k );
    } );
}

int main(){

    process(8760, 5000, 0);

    return 0;
}