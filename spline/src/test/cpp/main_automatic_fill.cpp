#include <iostream>
#include <fstream>
#include <math.h>
#include "BSpline.h"

void demo_bspline(int n_test);
double function(double x);

int main(char** argv, int argc) {

    int n_test = 36;          // used only for test
    //std::cin >> n_test;     //

    demo_bspline(n_test);
	system("pause");
}

double func(double x)
{
    return sin(x);
}

void demo_bspline(int n_test)
{
	Curve* curve = new BSpline();
    curve->set_steps(1000);

    for (int i = 0; i <= n_test; i++)
    {
        curve->add_way_point(Vector(i, func((double)i), 0));
    }

	std::cout << "nodes: " << curve->node_count() << std::endl;
	std::cout << "total length: " << curve->total_length() << std::endl;
    for (int i = 0; i < curve->node_count(); ++i)
    {
        std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
    }

    std::ofstream outfile("test(3)_lambda-1.txt");

    for (int i = 0; i < curve->node_count(); ++i)
		outfile << curve->node(i).toFile();

	outfile.close();

	delete curve;
}
