#include "mpi.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include "stdio.h"
#include "../../main/cpp/BSpline.h"
#include "../../main/cpp/atmsp.h"

ATMSP<double> parser;
ATMSB<double> byteCode;

void bspline(double *points, int N, int M, int rank, int size, int p, double lambda);

int main(int argc, char** argv) {
    clock_t t;
    double a, b, lambda = 0;
    int N, M, p;
    char func[128];

    int st = MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Status status;
    if (st != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, 2);
    }
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, 3);
    }

    if(rank == 0) {
        std::cout << "Enter your function: ";
        std::cin.getline(func, 128);
        std::cout << "Enter the segment and the number of internal points to approximate (a b N): ";
        std::cin >> a >> b >> N;
        std::cout << "Enter the number of steps: ";
        std::cin >> M;
		std::cout << "Enter the order: ";
		std::cin >> p;
		if (p > 3 && p < 7) {
			std::cout << "Enter the lambda parameter (" << -p * (p - 2) << " <= lambda <= 1): ";
			std::cin >> lambda;
			if (-p * (p - 2) > lambda || lambda > 1)
				lambda = 0;
		}
		else
			p = 4;
    }
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&lambda, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(func, 128, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    double points[10000] = { 0 };
    int len = (int)ceil((N - 1) / size) + 1;

    int sz = 0;

    for (int i = len * rank; (i < len*(rank + 1) + 3) && (i < N + 2); i++) {
	points[sz] = a + i * (b - a) / (N + 1);
        sz++;
    }

    size_t err = parser.parse(byteCode, func, "x");
    if (err) {
        std::cout << "Parsing failed with: " << parser.errMessage(err) << std::endl;

        st = MPI_Finalize();
        if (st != MPI_SUCCESS) {
            MPI_Abort(MPI_COMM_WORLD, 6);
        }
    }

    bspline(points, sz, M, rank, size, p, lambda);

    t = clock() - t;

    if(rank == size - 1) {
        printf ("It took me %f seconds.\n", ((float)t)/CLOCKS_PER_SEC);
    }

    return 0;
}

void bspline(double *points, int N, int M, int rank, int size, int p, double lambda) {
    int st;
    MPI_Status status;
    Curve* curve = new BSpline(p, lambda);
    curve->set_steps(M);

    for (int i = 0; i < N; i++) {
	byteCode.var[0] = points[i];
        curve->add_way_point(Vector(byteCode.var[0], byteCode.run(), 0));
    }

    int tk = 0;
    if (rank != 0) {
        st = MPI_Recv(&tk, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        if (st != MPI_SUCCESS) {
            MPI_Abort(MPI_COMM_WORLD, 4);
        }
    }

    std::ofstream outf_approx;

    if (rank == 0) {
        outf_approx.open("test_approx.txt");
    }
    else {
        outf_approx.open("test_approx.txt", std::ios::app);
    }

    for (int i = 0; i < curve->node_count(); ++i) {
        outf_approx << curve->node(i).toFile();
    }

    outf_approx.close();

    std::ofstream outf_func;

    if (rank == 0) {
        outf_func.open("test_func.txt");
    }
    else {
        outf_func.open("test_func.txt", std::ios::app);
    }

    double h = (points[N - 2] - points[1]) / (M * (N - 3));
    for (int i = 0; i <= M * (N - 3); ++i) {
        byteCode.var[0] =  points[1] + h*i;
        outf_func << byteCode.var[0] << " " << byteCode.run() << std::endl;
    }

    outf_func.close();

    st = MPI_Send(&tk, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    if (st != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, 4);
    }

    delete curve;

    st = MPI_Finalize();
    if (st != MPI_SUCCESS)
    {
        MPI_Abort(MPI_COMM_WORLD, 6);
    }
}
