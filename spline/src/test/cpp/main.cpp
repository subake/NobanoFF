#include <iostream>
#include <fstream>
#include "../../main/cpp/BSpline.h"

void demo_bspline();

int main(char** argv, int argc) {
    demo_bspline();
    system("pause");
}

void demo_bspline() {
    Curve* curve = new BSpline();
    curve->set_steps(1000); // generate 100 interpolate points between the last 4 way points
    curve->add_way_point(Vector(1, 1, 0));
    curve->add_way_point(Vector(1.5, 2.5, 0));
    curve->add_way_point(Vector(2, 3, 0));
    curve->add_way_point(Vector(2.5, 2.5, 0));
    curve->add_way_point(Vector(3, 2, 0));
    curve->add_way_point(Vector(3.5, 3, 0));
    curve->add_way_point(Vector(4, 6, 0));
    std::cout << "nodes: " << curve->node_count() << std::endl;
    std::cout << "total length: " << curve->total_length() << std::endl;
    for (int i = 0; i < curve->node_count(); ++i) {
 	std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
    }

    std::ofstream outfile("test.txt");
    for (int i = 0; i < curve->node_count(); ++i) {
 	outfile << curve->node(i).toFile();
    }
    outfile.close();

    delete curve;
}
