#pragma once


#include <vector>
#include <iostream>
#include "Event_queue.h"

#define BLACK 0
#define RED 1

#define VERTICAL 1
#define HORIZONTAL -1
#define INCLINED 0




class Orientation {
    double slope;
public:
    const int type;
    Orientation(int t) : slope{}, type{ t } {
        if (t == INCLINED) {
            std::cerr << "Missing slope argument" << std::endl;
        }
        else if (t == HORIZONTAL) {
            slope = 0;
        }
    }
    Orientation(double m) : slope{ m }, type{ (m == 0) ? HORIZONTAL : INCLINED } {}
    double value() {
        return slope;
    }
};


/**
 * checks if a event point lies inside the rectangle whose diagonal is the line segment
 */

bool box_contains(const Line_segment* ls, const Event_point& p);


/**
 * To find the slope of line segments. Returns an Orientation object which helps to identify if a line segment is horizontal,
 * vertical or inclined and retrieve the slope accordingly.
 */

Orientation slope(const Line_segment* ls);

/**
 * get the point where the sweeping line through an even point meets a line segment
 */
Event_point get_intercept(const Line_segment* ls, const Event_point& p, int type);

/**
 * returns the anticlockwise angle in radians of a line segment around the event point p
 */
double angle(const Line_segment* ls, const Event_point& ep);

/**
 * checks if a line segment is "less than" another line segment by comparing the order in which a sweeping line below
 * the event point p whould intersect the two segments. In case they intersect at the same point, comparrson is down based on
 * angles around p.
 */
bool line_segment_lt(const Line_segment* ls1, const Line_segment* ls2, const Event_point& p);

/**
 * returns an Event point which is either the intersection points of the two  line segments or an empty event point
 */
Event_point get_intersection(const Line_segment* ls1, const Line_segment* ls2);

/**
 * checks if a event point lies on, left or right of a line segment.
 */
int ls_position(const Line_segment* ls, const Event_point& p);



class Status_structure {
    
    struct Segment {
        Segment* left;
        Segment* right;
        Segment* parent;
        bool color;
        const Line_segment* key;
        Segment(const Line_segment* ls = nullptr) :
            left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ BLACK }, key{ ls } {}
    };

    
private:
    Segment* nil;
    Segment* root;

    
public:
    Status_structure() : nil{ new Segment() }, root{ nil } {}

    /**
     * returns the segment immediately to the left the event point p, nil otherwise
     */
    const Segment* find_left_neighbor(const Event_point& p) { return fln(p, root); }

    /**
     * returns the segment immediately to the right the event point p, nil otherwise
     */
    const Segment* find_right_neighbor(const Event_point& p) { return frn(p, root); }

    /**
    * returns the segment immediately to the left the segment s (predecessor), nil otherwise
    */
    const Segment* find_left_neighbor(const Segment* s);
    /**
     * returns the segment immediately to the right the segment s (successor), nil otherwise
     */
    
    const Segment* find_right_neighbor(const Segment* s);
    /**
     * returns the left most segment in the status structure which is present in U(p) U C(p).
     */
    
    const Segment* search_leftmost(std::vector<const Segment*>& vls1, std::vector<const Segment*>& vls2);
    /**
     * returns the right most segment in the status structure which is present in U(p) U C(p).
     */
    
    const Segment* search_rightmost(std::vector<const Segment*>& vls1, std::vector<const Segment*>& vls2);

    /**
    * returns a vector of segments that store the line segments having the event point as their lower endpoint
    */
    std::vector<const Segment*> get_lp(const Event_point& ep);
    /**
     * returns a vector of segments that store the line segments having the event point as their interior point
     */
    
    std::vector<const Segment*> get_cp(const Event_point& ep);
    /**
     * insert a line segment in the status structure which has the event point as its upper endpoint
     */
    

    void insert(const Line_segment* ls, const Event_point& p);
    void remove(const Segment*);
    bool empty() { return root == nil; }
    bool empty(const Segment* s) { return s == nil; }
    std::vector<const Segment*> inorder() {
        std::vector<const Segment*> vls;
        inorder(root, vls);
        return vls;
    }
    
    void show_tree() { preorder(root, 0); }

    ~Status_structure() { delete nilify(root); }

    
private:
    
    Segment* fln(const Event_point& p, Segment* s);
    
    Segment* frn(const Event_point& p, Segment* s);
    
    Segment* deconst(const Segment*);
    
    Segment* search(const Line_segment* ls, Segment* s);
    
    void inorder(Segment* x, std::vector<const Segment*>& vls);
    
    void preorder(Segment* x, int indent);

    Segment* tree_minimum(Segment* x);
    Segment* tree_maximum(Segment* x);
    void left_rotate(Segment* z);
    void right_rotate(Segment* z);
    void transplant(Segment* u, Segment* v);
    void insert_fixup(Segment* z);
    void remove_fixup(Segment* x);
    Segment* nilify(Segment* x);
};
