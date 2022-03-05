#include "status_structure.h"


bool operator==(const Line_segment& ls1, const Line_segment& ls2)
{
    return (ls1.uep == ls2.uep && ls1.lep == ls2.lep);
}

bool operator!=(const Line_segment& ls1, const Line_segment& ls2)
{
    return (ls1.uep != ls2.uep || ls1.lep != ls2.lep);
}


bool operator<(const Line_segment& ls1, const Line_segment& ls2)
{
    return (ls1.uep.x < ls2.uep.x || (ls1.uep.x == ls2.uep.x && slope(ls1) > slope(ls2)));
}


Status_structure::Segment* Status_structure::search(const Line_segment& ls)
{
    Segment* x = root;
    while (x != nil && x->key != ls) {
        if (ls < x->key) {
            x = x->left;
        }
        else x = x->right;
    }
    return x;
}


void Status_structure::preorder(Segment* x, int indent)
{
    if (x != nil) {
        for (int i = 0; i < indent; ++i) std::cout << "    ";
        
        if (x->color == RED) std::cout << "r";
        else std::cout << "b";

        std::cout << "[" << "(" << x->key.uep.x << ", " << x->key.uep.y << ")" << ", " << 
                    "(" << x->key.lep.x << ", " << x->key.lep.y << ")" << "]";

        std::cout << ": [";
        auto& segments = x->starting_segments;
        for (const auto& segment : segments) {
            std::cout << segment << ", ";
        }
        std::cout << "]" << std::endl;
        
        preorder(x->left, indent + 1);

        preorder(x->right, indent + 1);
    }
    else {
        for (int i = 0; i < indent; ++i) std::cout << "    ";
        std::cout << "nil" << std::endl;
    }
}






void Status_structure::insert(const Line_segment& ls, int seg)
{
    Segment* y = search(ls);
    if (y != nil) {
        if (seg != -1) {
            auto& segments = y->starting_segments;
            for (const auto& segment : segments) {
                if (segment == seg) return;
            }
            segments.push_back(seg);
        }
        return;
    }
    Segment* x = root;
    while (x != nil) {
        y = x;
        if (ls < x->key) x = x->left;
        else x = x->right;
    }
    Segment* z = new Segment(ls, seg);
    z->left = nil;
    z->right = nil;
    z->parent = y;
    z->color = RED;
    if (y == nil) root = z;
    else if (ls < y->key) y->left = z;
    else y->right = z;
    insert_fixup(z);
}




void Status_structure::remove(Segment* z)
{
    if (z == nil) return;
    Segment* y = z;
    Segment* x = z;
    bool y_org_color = y->color;
    if (z->left == nil) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = tree_minimum(z->right);
        y_org_color = y->color;
        x = y->right;
        if (y->parent == z) x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (y_org_color == BLACK)
        remove_fixup(x);
}





Status_structure::Segment* Status_structure::tree_minimum(Segment* x)
{
    while (x->left != nil) {
        x = x->left;
    }
    return x;
}






void Status_structure::left_rotate(Segment* x)
{
    Segment* y = x->right;
    x->right = y->left;
    if (y->left != nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}






void Status_structure::right_rotate(Segment* y)
{
    Segment* x = y->left;
    y->left = x->right;
    if (x->right != nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nil) root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}




void Status_structure::transplant(Segment* u, Segment* v)
{
    if (u->parent == nil) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}







void Status_structure::insert_fixup(Segment* z)
{
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Segment* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(z->parent->parent);
            }
        }
        else {
            Segment* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}








void Status_structure::remove_fixup(Segment* x)
{
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Segment* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(x->parent);
                x = root;
            }
        }
        else {
            Segment* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

Status_structure::Segment* Status_structure::nilify(Segment* x)
{
    if (x != nil) {
        x->left = nilify(x->left);
        x->right = nilify(x->right);
        delete x;
    }
    return nil;
}