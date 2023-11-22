#include <cmath>
#include <vector>
#include <omp.h>

typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));

constexpr double4_t d4zero {0.0, 0.0, 0.0, 0.0};

static inline double sum_v(const double4_t& a) {
    return a[0] + a[1] + a[2] + a[3];
}

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
    
    // normalize the input data
    double * normalized_data = new double[ny*nx]{0.0};

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
    const int block_size = 4;
    int col = (nx + block_size - 1) / block_size;

    std::vector<double4_t> normalized_data_vec(ny * col);
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < col; j++) {
            for (int k = 0; k < block_size; k++) {
                int idx = j*block_size + k;
                normalized_data_vec[i*col + j][k] = idx < nx ? normalized_data[idx + i*nx] : 0.0;
            }
        }
    }

    // compute the correlation
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {
            double4_t block_product = d4zero;
            for (int k = 0; k < col; k++) {
                block_product += normalized_data_vec[i*col + k] * normalized_data_vec[j*col + k];
            }
            result[i + j*ny] = sum_v(block_product);
        }
    }

    delete[] normalized_data;
}