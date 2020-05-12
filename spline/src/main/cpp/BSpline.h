#ifndef _H__BSPLINE_H
#define _H__BSPLINE_H

#include "Curve.h"

class BSpline : public Curve {
public:
    BSpline();
	BSpline(int p, double lambda);
    virtual ~BSpline();

protected:
    virtual void _on_way_point_added();

protected:
	int p;
	double lambda;

protected:
    Vector interpolate_4_lambda(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3);
    Vector interpolate_5_lambda(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3);
    Vector interpolate_6_lambda(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3);
};

#endif
