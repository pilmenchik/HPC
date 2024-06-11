#include <cstdlib>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <iomanip> 

using namespace std;


int main(int argc, char* argv[]) {
    
    const int num_matrix = 2000;
    int num_Threads = 8;
    //створення динамічного масиву
    double** a = new double* [num_matrix];
    for (int i = 0; i < num_matrix; ++i)
    {
        a[i] = new double[num_matrix];
    }
    srand((unsigned)time(NULL));
    //заповнення рандомними числами
    for (int i = 0; i < num_matrix; i++) {
        for (int j = 0; j < num_matrix; j++) {
            a[i][j] = 20 + rand() % 100;
        }
    }
    //створення допоміжного динамічного масиву
    double** r = new double* [num_matrix];
    for (int i = 0; i < num_matrix; ++i)
    {
        r[i] = new double[num_matrix];
    }
    //створення допоміжного динамічного масиву
    double** q = new double* [num_matrix];
    for (int i = 0; i < num_matrix; ++i)
    {
        q[i] = new double[num_matrix];
    }

    int k, i, j;
    omp_set_num_threads(num_Threads);

    double start = omp_get_wtime();
        for (k = 0; k < num_matrix; k++) {
            r[k][k] = 0;
#pragma omp parallel for shared(a, r, k, i)
            for (i = 0; i < num_matrix; i++) {
                r[k][k] = r[k][k] + a[i][k] * a[i][k];
            }
            r[k][k] = sqrt(r[k][k]);

#pragma omp parallel for shared(a, q, r, k, i)
            for (i = 0; i < num_matrix; i++) {
                q[i][k] = a[i][k] / r[k][k];
            }
#pragma omp parallel for shared(a, q, r, k, j)
            for (j = k + 1; j < num_matrix; j++) {
                r[k][j] = 0;

#pragma omp parallel for shared(a, q, r, i, k, j)
                for (i = 0; i < num_matrix; i++) {
                    r[k][j] += q[i][k] * a[i][j];
                }
#pragma omp parallel for shared(a, q, r, i,k,j)
                for (i = 0; i < num_matrix; i++) {
                    a[i][j] = a[i][j] - r[k][j] * q[i][k];
                }
            }
        }
        
    int for_end = 0;
    double end = omp_get_wtime();
    double time = end - start;
    cout << "\n";
    cout << "Matrix: " << num_matrix << " x " << num_matrix;
    cout << "\n";
    cout << "Number of threads " << num_Threads;
    cout << "\n";
    cout << "Time of execution : " << setprecision(8) << time << endl;

    return 0;

}