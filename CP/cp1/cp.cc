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
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {
            double sum_i = 0.0;
            double sum_j = 0.0;
            double sum_square_i = 0.0;
            double sum_square_j = 0.0;
            double sum_product_ij = 0.0;
            for (int k = 0; k < nx; k++) {
                double data_i = data[k + i*nx];
                double data_j = data[k + j*nx];
                sum_i += data_i;
                sum_j += data_j;
                sum_square_i += pow(data_i, 2);
                sum_square_j += pow(data_j, 2);
                sum_product_ij += data_i * data_j;
            }
            double mean_i = sum_i / nx;
            double mean_j = sum_j / nx;
            double variance_i = sum_square_i - nx * pow(mean_i, 2);
            double variance_j = sum_square_j - nx * pow(mean_j, 2);
            result[i + j*ny] = (sum_product_ij - nx * mean_i * mean_j) / sqrt(variance_i * variance_j);
        }
    }
}