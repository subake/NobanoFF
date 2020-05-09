#ifndef _H__CURVE_H
#define _H__CURVE_H

#include "Vector.h"
#include <vector>
#include <cassert>

class Curve {
public:
    Curve();
    virtual ~Curve();

protected:
    std::vector<Vector> _way_points;
public:
    void add_way_point(const Vector& point);
    void compute(int argc, char** argv);
    void clear();

protected:
    void add_node(const Vector& node);
    virtual void _on_way_point_added() = 0;
    virtual void _compute_interpolation(int argc, char** argv) = 0;

protected:
    std::vector<Vector> _nodes;
    std::vector<double> _distances;
public:
    Vector node(int i) const { return _nodes[i]; }
    std::vector<Vector> nodes() const { return _nodes; };
    double length_from_starting_point(int i) const { return _distances[i]; }
    bool has_next_node(int i) const { return static_cast<int>(_nodes.size()) > i; }
    int node_count() const {  return static_cast<int>(_nodes.size()); }
    bool is_empty() { return _nodes.empty(); }
    double total_length() const {
	assert(!_distances.empty());
	return _distances[_distances.size() - 1];
    }

protected:
    int _steps;
public:
    void increment_steps(int steps) { _steps+=steps; }
    void set_steps(int steps) { _steps = steps; }
};

#endif
