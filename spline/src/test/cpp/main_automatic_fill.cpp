#include <iostream>
#include <fstream>
#include <cmath>
#include "BSpline.h"

#define _USE_MATH_DEFINES

void demo_bspline(int n_test);
double function(double x);

int main(char** argv, int argc)
{
    int n_test = 36;          // used only for test
    //std::cin >> n_test;     //

    demo_bspline(n_test);
	system("pause");
}

double test(double x)
{
    double a = 0.05;
    return 0.5 * M_1_PI * sqrt(a * (2 + a)) / (1 + a - cos(x));
}

void demo_bspline(int n_test)
{
	Curve* curve = new BSpline();
    curve->set_steps(1000);

    for (int i = (-1) * n_test / 2; i <= n_test / 2; i++)
    {
        double x = 2 * M_PI * i/n_test;
        curve->add_way_point(Vector(x, test(x), 0));
    }

	std::cout << "nodes: " << curve->node_count() << std::endl;
	std::cout << "total length: " << curve->total_length() << std::endl;
    for (int i = 0; i < curve->node_count(); ++i)
    {
        std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
    }

    std::ofstream outfile("test(4)_lambda1.txt");

    for (int i = 0; i < curve->node_count(); ++i)
		outfile << curve->node(i).toFile();

	outfile.close();

	delete curve;
}
