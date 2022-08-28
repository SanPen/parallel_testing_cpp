
#ifndef NEWTON_PAR_FOR_H
#define NEWTON_PAR_FOR_H

#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <cmath>
#include <mutex>
#include <iostream>

#include <windows.h>
#include <winbase.h>


template<typename T>
struct ParallelChunkData {
    std::vector<T> indices;
    std::vector<T> indices_map;

    ParallelChunkData(){};

    ParallelChunkData(const std::vector<T>& idx, const std::vector<T>& idx_map):
    indices(idx), indices_map(idx_map)
    { }

    void add(T idx, T idx_m){
        indices.push_back(idx);
        indices_map.push_back(idx_m);
    }
};

/**
 * get the number of threads that the system can handle
 * @return unsigned int
*/
unsigned int getThreadNumber();

template<typename T>
std::vector<T> getIndexMapping(const std::vector<T>& indices){
    std::vector<T> res(indices.size());
    for(size_t i=0; i < indices.size(); ++i) res[i] = i;
    return res;
}

template<typename T>
std::vector<T> arrange(size_t n){
    std::vector<T> res(n);
    for(size_t i=0; i < n; ++i) res[i] = i;
    return res;
}

/**
 * determine the partition plan
 * @tparam Index type of number (usually some kind of int)
 * @param nt number of time steps
 * @return vector of start-end pairs
 */
template<typename Index>
std::vector<std::pair<Index, Index>> prepareParallelChunks(Index nt, unsigned int nb_threads=0){
    // Estimate number of threads in the pool
    std::size_t nth = nb_threads == 0 ? getThreadNumber(): nb_threads;

    // prepare the execution plan
    std::vector<std::pair<Index, Index>> bounds;
    Index a, b, share;
    share = nt / nth;
    a;
    b = 0;
    for (Index n=0; n < nth; ++n){
        a = b;
        b += share;
        if (n == (nth - 1)) b += (nt - b); // add the rest if we're on the last one
        bounds.emplace_back(a, b);
    }

    return bounds;
}

template<typename T>
std::vector<ParallelChunkData<T>> prepareParallelChunks(std::vector<T> indices, int nb_threads=0){

    // Estimate number of threads in the pool
    std::size_t nth = nb_threads == 0 ? getThreadNumber(): nb_threads;

    // declare the results
    std::vector<ParallelChunkData<T>> chunks;

    if (nth == 1){
        // if only one core (or less), the results are the inputs
        chunks.emplace_back(indices, indices);
    } else {
        T a, b, share, nt;
        nt = indices.size();
        share = nt / nth;
        if (share == 0) {share = 1; nth = nt;} // when there are more cores than indices
        b = 0;
        for (size_t n=0; n < nth; ++n){
            a = b;
            b += share;
            if (n == (nth - 1)) b += (nt - b); // add the rest if we're on the last one

            ParallelChunkData<T> chunk;
            for (T i=a; i<b; ++i){
                chunk.add(indices[i], i);
            }
            chunks.push_back(chunk);
        }
    }


    return chunks;
}

/**
 * parallel for
 * @tparam Index integer type
 * @tparam Callable function type
 * @param start start index of the loop
 * @param end final +1 index of the loop
 * @param func function to evaluate
 * @param nb_threads number of threads, if zero, it is determined automatically
 */
template<typename Index, typename Callable>
static void ParallelFor(Index start, Index end, Callable func, unsigned nb_threads=0) {

    // Estimate number of threads in the pool
    if (nb_threads == 0) nb_threads = getThreadNumber();

    // Size of a slice for the range functions
    Index n = end - start + 1;
    Index slice = (Index) std::round(n / static_cast<double> (nb_threads));
    slice = slice > Index(1) ? slice: Index(1);  // std::max(slice, Index(1));

    // [Helper] Inner loop
    auto launchRange = [&func] (int k1, int k2) {
        for (Index k = k1; k < k2; k++) {
            func(k);
        }
    };

    // Create pool and launch jobs
    std::vector<std::thread> pool;
    pool.reserve(nb_threads);
    Index i1 = start;
    Index i2 = (start + slice) < end ? (slice + start) : end; //std::min(start + slice, end);

    for (unsigned i = 0; i + 1 < nb_threads && i1 < end; ++i) {
        pool.emplace_back(launchRange, i1, i2);

        DWORD_PTR dw = SetThreadAffinityMask(pool.back().native_handle(), DWORD_PTR(1) << i);
        if (dw == 0) {
            DWORD dwErr = GetLastError();
            std::cerr << "SetThreadAffinityMask failed, GLE = " << dwErr << '\n';
        }

        i1 = i2;
        i2 = (i2 + slice) < end ? (i2 + slice): end; //std::min(i2 + slice, end);
    }

    if (i1 < end) {
        pool.emplace_back(launchRange, i1, end);
    }

    // Wait for jobs to finish
    for (std::thread &t : pool) {
        if (t.joinable()) {
            t.join();
        }
    }
}


/**
 * Serial version for easy comparison
 * @tparam Index
 * @tparam Callable
 * @param start
 * @param end
 * @param func
 */
template<typename Index, typename Callable>
static void SequentialFor(Index start, Index end, Callable func) {
    for (Index i = start; i < end; i++) {
        func(i);
    }
}

/**
 * Timer class
 */
class Timer {
    std::thread _th;
    bool _running = false;

public:

    void start(const std::chrono::milliseconds &interval, const std::function<void(void)>& fn );

    void stop();
};




#endif //NEWTON_PAR_FOR_H
