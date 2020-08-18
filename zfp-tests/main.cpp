#include <iostream>
#include <cmath>
#include <iomanip>

#include "zfp.h"

#define PRECISION 7

int main() {

    int nx = 2, ny = 5, nz = 5;
    double test_array[nz][ny][nx]; // color varies the fastest

    std::cout << "\noriginal array:" << std::endl;
    for (int j = 0; j < nz; j++) {
        for (int i = 0; i < ny; i++) {
            for (int z = 0; z < nx; z++) {
                double x = 2.0 * i / nx;
                double y = 3.0 * j / ny;
                test_array[j][i][z] = exp(-(x * x + y * y + z * z));
                std::cout << std::setprecision(16) << test_array[j][i][z] << std::endl;
            }
        }
//        std::cout << std::endl;
    }

    printf("\n===============================\n");
    printf("\noriginal array size = %lu bytes\n", sizeof(test_array));

    //zfp setup for dims and type
    uint dims = 3;
    zfp_type type = zfp_type_double;
    zfp_field *compressed_field = zfp_field_3d(&test_array, type, nx, ny, nz);
    zfp_stream *zfp = zfp_stream_open(NULL);

    //set zfp mode
    uint target_precision = 7, actual_precision;
    actual_precision = zfp_stream_set_precision(zfp, PRECISION);

    std::cout << "Requested Precision: " << PRECISION << std::endl << "Actual Precision: " << actual_precision << std::endl;

    //get min ~safe~ buffer size
    size_t bufsize = zfp_stream_maximum_size(zfp, compressed_field);
    if (bufsize == 0) {
        std::cerr << "Error occurred! bufsize was returned as 0 by zfp" << std::endl;
        return 1;
    }

    //allocate bit stream
    uchar *buffer = new uchar[bufsize];
    bitstream *stream = stream_open(buffer, bufsize);
    zfp_stream_set_bit_stream(zfp, stream);

    //skip using OpenMP or CUDA
    size_t compressed_size = zfp_compress(zfp, compressed_field);
    if (compressed_size == 0) {
        std::cerr << "Returned compressed size was 0!" << std::endl;
        return 1;
    } else {
        std::cout << "Returned size was " << compressed_size << " bytes \n" << std::endl;
    }

    //now decompress the array
    zfp_stream_rewind(zfp); // Move buf to the start
    double decompressed_array[nz][ny][nx];
    zfp_field *decompressed_field = zfp_field_3d(&decompressed_array, type, nx, ny, nz);
    size_t decompressed_size = zfp_decompress(zfp, decompressed_field);
    if (decompressed_size == 0) {
        std::cerr << "Decompressed size was 0!" << std::endl;
    } else {
        std::cout << "Decompressed size was " << decompressed_size << std::endl;
    }
    int t = 0;
    // Check to see the result
    for (int j = 0; j < nz; j++) {
        for (int i = 0; i < ny; i++) {
            for (int z = 0; z < nx; z++) {
//                if (decompressed_array[j][i][z] != 0.)
//                {
                    std::cout << std::setprecision(16) << decompressed_array[j][i][z] << std::endl;
                    t += 1;
//                }
            }
        }
    }
//    std::cout << t;


    return 0;
}
