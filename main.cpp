#include <fstream>
#include "Event_queue.h"
#include "Status_structure.h"
#include <chrono>

Event_queue eq;
Status_structure ss;

std::string filename = "";



/**
 *prints the intersection point along with the segments intersecting it
 */
void print_intersection(const Event_point& p,
    std::vector<const Line_segment*>& up,
    std::vector<const Line_segment*>& lp,
    std::vector<const Line_segment*>& cp)
{
    std::cout << "(" << p.x << ", " << p.y << "): [";
    for (const auto& x : up) {
        std::cout << x->number << ", ";
    }
    for (const auto& x : lp) {
        std::cout << x->number << ", ";
    }
    for (const auto& x : cp) {
        std::cout << x->number << ", ";
    }
    std::cout << "]" << std::endl;
    std::ofstream ofs(std::string("output/")+filename, std::ios::app);
    ofs << p.x << " " << p.y << std::endl;
}


/**
 * finds new intersection points
 */
void find_new_events(const Line_segment* ls_left, const Line_segment* ls_right, const Event_point& p)
{
    Event_point ip = get_intersection(ls_left, ls_right);
    if (ip.null_ep) return;
    if (p < ip) {
        if (eq.empty(eq.search(ip))) {
            eq.insert(ip);
        }
    }
}



/**
 * handles the next event point
 */

void handle_event_point(const Event_point& p, std::vector<const Line_segment*>& up)
{
    auto seg_lp = ss.get_lp(p);
    auto seg_cp = ss.get_cp(p);
    std::vector<const Line_segment*> lp, cp;
    for (auto& x : seg_lp) {
        lp.push_back(x->key);
        ss.remove(x);
    }
    for (auto& x : seg_cp) {
        cp.push_back(x->key);
        ss.remove(x);
    }
    if ((up.size() + cp.size() + lp.size()) > 1) {
        print_intersection(p, up, lp, cp);
    }
    for (auto& x : up) {
        cp.push_back(x);
    }
    auto vls1 = ss.inorder();
    for (auto& x : cp) {
        ss.insert(x, p);
    }
    auto vls2 = ss.inorder();
    if (cp.size() == 0) {
        auto seg_ls_left = ss.find_left_neighbor(p);
        auto seg_ls_right = ss.find_right_neighbor(p);
        if (!ss.empty(seg_ls_left) && !ss.empty(seg_ls_right)) {
            find_new_events(seg_ls_left->key, seg_ls_right->key, p);
        }
    }
    else {
        auto seg_ls_d = ss.search_leftmost(vls1, vls2);
        auto seg_ls_dd = ss.search_rightmost(vls1, vls2);
        auto seg_ls_left = ss.find_left_neighbor(seg_ls_d);
        if (!ss.empty(seg_ls_left)) {
            find_new_events(seg_ls_left->key, seg_ls_d->key, p);
        }
        auto seg_ls_right = ss.find_right_neighbor(seg_ls_dd);
        if (!ss.empty(seg_ls_right)) {
            find_new_events(seg_ls_dd->key, seg_ls_right->key, p);
        }
    }
}


/**
 * finds intersection points among the line segements from s
 */
void find_intersections(const std::vector<Line_segment>& s)
{
    for (auto& ls : s) {
        eq.insert(ls.uep, &ls);
        eq.insert(ls.lep);
    }
    while (!eq.empty()) {
        auto event_ptr = eq.next();
        auto p = event_ptr->key;
        auto up = event_ptr->starting_segments;
        eq.remove(event_ptr);
        handle_event_point(p, up);
    }
}


/**
 * helper function for file handling during io
 */
void get_filename(std::string s) {
    int i = 0, last_back_slash = 0;
    for (char c : s) {
        if (c == '/') {
            last_back_slash = i;
        }
        ++i;
    }
    std::string str = "";
    for (int j = last_back_slash+1; j < i; ++j) {
        str += s[j];
    }
    filename = str;
}

/**
 * main fn takes input as a txt file and creates an output file in the 'output' directory
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: intersection <filename>" << std::endl;
        return 0;
    }
    else {
        std::ifstream ifs(argv[1]);
        if (!ifs.is_open()) {
            std::cerr << "Couldn't open " << argv[1] << std::endl;
        }
        else {
            get_filename(argv[1]);
            std::ofstream ofs(std::string("output/") + filename);

            int n;
            ifs >> n;
            std::vector<Line_segment> s;
            for (int i = 0; i < n; ++i) {
                double ux, uy, lx, ly;
                ifs >> ux >> uy >> lx >> ly;
                Event_point uep{ ux, uy };
                Event_point lep{ lx, ly };
                if (lep == uep || lep < uep) {
                    std::cerr << "<<Lower endpoint must be below upper endpoint>>\n";
                    return 0;
                }
                Line_segment ls{ i, uep, lep };
                s.push_back(ls);
            }
            auto start = std::chrono::high_resolution_clock::now();
            find_intersections(s);
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "Time taken by find_intersection(): " << duration.count() << "ms" << std::endl;
        }
    }
    return 0;

}