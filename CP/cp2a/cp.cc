#include <cmath>
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
            double norm_factor = sqrt(sum_square_i);
            for (int j = 0; j < nx; j++) {
                normalized_data[j + i*nx] /= norm_factor;
            }
        }
    }

    // compute the correlation
    const int block_size = 4;
    int col = nx / block_size;

    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {

            double block_sum_product_ij[block_size]{0.0};

            // the first (n/4)*4 elements
            for (int k = 0; k < col; k++) {
                for (int m = 0; m < block_size; m++) {
                    double data_i = normalized_data[i*nx + k*block_size + m];
                    double data_j = normalized_data[j*nx + k*block_size + m];
                    block_sum_product_ij[m] += data_i * data_j;
                }
            }

            // the last n%4 elements
            for (int k = col*block_size; k < nx; k++) {
                double data_i = normalized_data[i*nx + k];
                double data_j = normalized_data[j*nx + k];
                block_sum_product_ij[k - col*block_size] += data_i * data_j;
            }

            double sum_product_ij = 0.0;
            for (int m = 0; m < block_size; m++) {
                sum_product_ij += block_sum_product_ij[m];
            }

            result[i + j*ny] = sum_product_ij;
        }
    }

    delete[] normalized_data;
}