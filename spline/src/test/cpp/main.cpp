#include <iostream>
#include <fstream>
#include <cmath>
#include "BSpline.h"
#include "atmsp.h"

ATMSP<double> parser;
ATMSB<double> byteCode;

void bspline(double *points, int N, int M, int argc, char** argv);

int main(int argc, char** argv) {
	double a, b;
	int N, M;
	std::string func;
	std::cout << "Enter your function: ";
	std::getline(std::cin, func);
	std::cout << "Enter the segment and the number of internal points to approximate (a b N): ";
	std::cin >> a >> b >> N;
	std::cout << "Enter the number of steps: ";
	std::cin >> M;

	double points[1000] = { 0 };
	for (int i = 0; i <= N + 1; ++i)
		points[i] = a + i * (b - a) / (N + 1);

	size_t err = parser.parse(byteCode, func, "x");
	if (err)
		std::cout << "Parsing failed with: " << parser.errMessage(err) << std::endl;

    bspline(points, N, M, argc, argv);
    system("pause");
}

void bspline(double *points, int N, int M, int argc, char** argv) {
    Curve* curve = new BSpline();
    curve->set_steps(M);

    for (int i = 0; i <= N + 1; i++) {
		byteCode.var[0] = points[i];
        curve->add_way_point(Vector(byteCode.var[0], byteCode.run(), 0));
    }
    curve->compute(argc, argv);

    std::cout << "nodes: " << curve->node_count() << std::endl;
    std::cout << "total length: " << curve->total_length() << std::endl;
    for (int i = 0; i < curve->node_count(); ++i) {
        std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
    }

    std::ofstream outf_approx("test_approx.txt");

    for (int i = 0; i < curve->node_count(); ++i) {
	outf_approx << curve->node(i).toFile();
    }

    outf_approx.close();

	std::ofstream outf_func("test_func.txt");

	double h = (points[N] - points[1]) / (M * (N - 1));
	for (int i = 0; i <= M * (N - 1); ++i) {
		byteCode.var[0] =  points[1] + h*i;
		outf_func << byteCode.var[0] << " " << byteCode.run() << std::endl;
	}

	outf_func.close();

    delete curve;
}
