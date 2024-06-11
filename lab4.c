#include <mpi.h>
#include <stdio.h>
#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))

int main(int argc, char **argv)
{
	int i, j;
	double array_A[3][3] = {{3.1, 1.3, 0.5}, {-1.7, 5.1, 2.3}, {4.7, 2.9, -1.2}};
	int n = LEN(array_A);
	int m = LEN(array_A[0]);
	double array_B[3] = {1.0, 2.0, 3.0};
	int k = LEN(array_B);
	int number_column = 3;
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0)
	{
		//array_A
		printf("Кількість процесів: %d \n", world_size);
		printf("Кількість стовпчиків матриці: %d \n", m);
		printf("Кількість рядків матриці: %d \n", n);
		printf("Матриця: \n");
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
			{
				printf("%f ", array_A[i][j]);
			}
			printf("\n");
		}

		printf("Вектор: \n");
		for (int i = 0; i < k; i++)
		{
			printf("%f ", array_B[i]);
		}
		printf("\n");
		printf("Стовпців містить 1 процес: %d \n", number_column);
	}

	double array_C[k];
	MPI_Bcast(array_B, k, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	int r = number_column * n;
	double rbuf[r];
	MPI_Scatter(array_A, r, MPI_DOUBLE, rbuf, r, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	double result[number_column];
	i = 0;
	int e;
	for (e = 0; e < number_column; e++)
	{
		double b = 0;
		for (int j = 0; j < k; j++)
		{
			b += array_B[j] * rbuf[i];
			i += 1;
		}
		result[e] = b;
	}

	MPI_Gather(result, number_column, MPI_DOUBLE, array_C, number_column, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (world_rank == 0)
	{
		printf("Відповідь: \n");
		for (int i = 0; i < k; i++)
		{
			printf("%f \n", array_C[i]);
		}
	}

	MPI_Finalize();
	return 0;
}