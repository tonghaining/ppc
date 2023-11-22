#include <cmath>
#include <vector>
#include <omp.h>
#include <immintrin.h>

typedef double double8_t __attribute__ ((vector_size (8 * sizeof(double))));

constexpr double8_t d8zero {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static inline double sum_v(const double8_t& a) {
    return a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7];
}

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result){

    // normalize the input data
    double * normalized_data = new double[ny*nx]{0.0};

    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < ny; i++) {
        // normalize the input rows so that each row has the arithmetic mean of 0
        double sum_i = 0.0;
        for (int j = 0; j < nx; j++) {
            sum_i += data[j + i*nx];
        }
        double mean_i = sum_i / nx;
        for (int j = 0; j < nx; j++) {
            normalized_data[j + i*nx] = data[j + i*nx] - mean_i;
        }
        
        // normalize the input rows so that for each row the sum of the squares of the elements is 1
        double sum_square_i = 0.0;
        for (int j = 0; j < nx; j++) {
            sum_square_i += pow(normalized_data[j + i*nx], 2);
        }
        if (sum_square_i != 0) {
            double norm_factor = std::sqrt(sum_square_i);
            for (int j = 0; j < nx; j++) {
                normalized_data[j + i*nx] /= norm_factor;
            }
        }
    }

    // vectorize the input data
    const int block_size = 8;
    const int ny_blocks = (ny + block_size - 1) / block_size;
    const int nx_blocks = (nx + block_size - 1) / block_size;

    // calculate ny after padding
    const int padded_ny = block_size * ny_blocks;

    std::vector<double8_t> normalized_data_vec(padded_ny * nx_blocks);
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx_blocks; j++) {
            for (int k = 0; k < block_size; k++) {
                int idx = j*block_size + k;
                normalized_data_vec[i*nx_blocks + j][k] = idx < nx ? normalized_data[idx + i*nx] : 0.0;
            }
        }
    }
    // padding
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = ny; i < padded_ny; i++){
        for (int j = 0; j < nx_blocks; j++){
            normalized_data_vec[j + nx_blocks * i] = d8zero;
        }
    }

    delete[] normalized_data;

    // compute the correlation
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < ny_blocks; i++){
        for (int j = i; j < ny_blocks; j++){
            double8_t block_product[block_size * block_size];
            // initializing
            for (int block = 0; block < block_size * block_size; block++){
                block_product[block] = d8zero;
            }

            // compute the block inner product
            for (int k = 0; k < nx_blocks; k++) {
                for (int row_id = 0; row_id < block_size; row_id++) {
                    double8_t row_vec = normalized_data_vec[(block_size * i + row_id) * nx_blocks + k];
                    for (int col_id = 0; col_id < block_size; col_id++) {
                        double8_t col_vec = normalized_data_vec[(block_size * j + col_id) * nx_blocks + k];
                        block_product[row_id * block_size + col_id] += row_vec * col_vec;
                    }
                }
            }

            // output the result
            for (int row_id = 0; row_id < block_size; row_id++){
                for (int col_id = 0; col_id < block_size; col_id++){
                    int source_i = block_size * i + row_id;
                    int source_j = block_size * j + col_id;
                    if (source_i < ny && source_j < ny){
                        result[source_i * ny + source_j] = sum_v(block_product[row_id * block_size + col_id]);
                    }
                }
            }
        }
    }
}