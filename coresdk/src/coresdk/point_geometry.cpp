//
//  point_geometry.cpp
//  splashkit
//
//  Created by Andrew Cain on 19/08/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include "point_geometry.h"

#include "random.h"
#include "graphics.h"
#include "vector_2d.h"

#include "geometry.h"

#include "utility_functions.h"

#include <cmath>

// Used in checking point similarities
#define SMALL 0.9
// smallest positive value: less than that to be considered zero
#define EPS   0.01
// and its square
#define EPSEPS 0.0001

point_2d point_at(float x, float y)
{
    point_2d result;
    result.x = x;
    result.y = y;
    return result;
}

point_2d point_at_origin()
{
    return point_at(0, 0);
}

point_2d point_offset_by(const point_2d &start_point, const vector_2d &offset)
{
    return point_at(start_point.x + offset.x, start_point.y + offset.y);
}

string point_to_string(const point_2d &pt)
{
    return "Pt @" + to_string(pt.x) + ":" + to_string(pt.y);
}

point_2d random_screen_point()
{
    return random_window_point(current_window());
}

point_2d random_window_point(window wind)
{
    point_2d result;
    result.x = rnd() * screen_width();
    result.y = rnd() * screen_height();
    return result;
}

point_2d random_bitmap_point(bitmap bmp)
{
    point_2d result;
    result.x = rnd() * bitmap_width(bmp);
    result.y = rnd() * bitmap_height(bmp);
    return result;
}

bool point_in_triangle(const point_2d &pt, const triangle &tri)
{
    vector_2d v0, v1, v2;
    vector_2d a, b, c, p;
    float dot00, dot01, dot02, dot11, dot12;
    float inv_denom, u, v;
    
    //Convert Points to vectors
    p = vector_to_point(pt);
    a = vector_to_point(tri.points[0]);
    b = vector_to_point(tri.points[1]);
    c = vector_to_point(tri.points[2]);
    
    // Compute vectors
    v0 = vector_subtract(c, a);
    v1 = vector_subtract(b, a);
    v2 = vector_subtract(p, a);
    
    // Compute dot products
    dot00 = dot_product(v0, v0);
    dot01 = dot_product(v0, v1);
    dot02 = dot_product(v0, v2);
    dot11 = dot_product(v1, v1);
    dot12 = dot_product(v1, v2);
    
    // Compute barycentric coordinates
    if (dot00 * dot11 - dot01 * dot01 == 0)
        return false;
    
    inv_denom = 1 / (dot00 * dot11 - dot01 * dot01);
    u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
    v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
    
    // Check if point is in triangle
    return ((u > 0) and (v > 0) and (u + v < 1));
}

bool point_in_rectangle(const point_2d &pt, const rectangle &rect)
{
    if (pt.x < rectangle_left(rect)) return false;
    else if (pt.x > rectangle_right(rect)) return false;
    else if (pt.y < rectangle_top(rect)) return false;
    else if (pt.y > rectangle_bottom(rect)) return false;
    else return true;
}

bool same_point(const point_2d &pt1, const point_2d &pt2)
{
    return static_cast<int>(pt1.x) == static_cast<int>(pt2.x) and static_cast<int>(pt1.y) == static_cast<int>(pt2.y);
}

bool point_in_circle(const point_2d &pt, const circle &c)
{
    return point_point_distance(c.center, pt) <= abs(c.radius);
}

bool point_on_line(const point_2d &pt, const line &l)
{
    auto simple_comparison_x_same = [&] ()
    {
        float min_y, max_y;
        
        min_y = MIN(l.start_point.y, l.end_point.y);
        max_y = MAX(l.start_point.y, l.end_point.y);
    
        return (pt.x >= l.start_point.x - SMALL) and (pt.x <= l.start_point.x + SMALL) and (pt.y >= min_y) and (pt.y <= max_y);
    };
    
    auto simple_comparison_y_same = [&] ()
    {
        float min_x, max_x;
    
        min_x = MIN(l.start_point.x, l.end_point.x);
        max_x = MAX(l.start_point.x, l.end_point.x);
    
        return (pt.y >= l.start_point.y - SMALL) and (pt.y <= l.start_point.y + SMALL) and (pt.x >= min_x) and (pt.x <= max_x);
    };
    
    float sq_line_mag, lx, ly, m, c;
    
    //Lines Magnitude must be at least 0.0001
    sq_line_mag = line_length_squared(l);
    if (sq_line_mag < 1.0)
    {
        return same_point(pt, l.start_point) or same_point(pt, l.end_point);
    }
    
    //Obtain the other variables for the Line Algorithm
    if ( l.end_point.x == l.start_point.x )
    {
        return simple_comparison_x_same();
    }
    if ( l.end_point.y == l.start_point.y)
    {
        return simple_comparison_y_same();
    }
    
    m = (l.end_point.y - l.start_point.y) / (l.end_point.x - l.start_point.x);
    c = l.start_point.y - (m * l.start_point.x);
    
    ly = (m * pt.x) + c;
    lx = (pt.y - c) / m;
    
    return (lx >= pt.x - SMALL) and
        (lx <= pt.x + SMALL) and
        (ly >= pt.y - SMALL) and
        (ly <= pt.y + SMALL) and
        point_in_rectangle(pt, rectangle_around(l));
}

/**
 *  Returns True of `pt1` is at the same point as `pt2`.
 */
bool same_point(const point_2d &pt1, const point_2d &pt2);

/**
 *  Returns the angle between two points in degrees.
 */
float calculate_angle_between(const point_2d &pt1, const point_2d &pt2);

/**
 *  Returns the distance between two points.
 */
float point_point_distance(const point_2d &pt1, const point_2d pt2)
{
    vector_2d temp = vector_point_to_point(pt1, pt2);
    return vector_magnitude(temp);
}
