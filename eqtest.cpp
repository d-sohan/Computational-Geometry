#include "Event_queue.h"


/**
 * This file is meant for testing the Event_queue data structure.
 */






void handle(const Event_point& ep, const std::vector<int>& start_seg)
{
    std::cout << "(" << ep.x << ", " << ep.y << "): [";
    for (auto seg : start_seg) {
        std::cout << seg << ", ";
    }
    std::cout << "]" << std::endl;
}




int main()
{
    std::cout << "No of lines? ";
    int n;
    std::cin >> n;
    Event_queue eq;
    for (int i = 0; i < n; ++i) {
        std::cout << "line " << i << " :\n";
        int ux, uy, lx, ly;
        std::cout << "Upper endpoint? ";
        std::cin >> ux >> uy;
        std::cout << "Lower endpoint? ";
        std::cin >> lx >> ly;
        Event_point uep{ ux, uy };
        Event_point lep{ lx, ly };
        if (lep == uep || lep < uep) {
            std::cerr << "<<Lower endpoint must be below upper endpoint>>\n";
            return 0;
        }
        eq.insert(uep, i);
        eq.insert(lep);
    }
    while (!eq.empty()) {
        auto event_ptr = eq.next();
        auto ep = event_ptr->key;
        auto v = event_ptr->starting_segments;
        eq.remove(event_ptr);
        handle(ep, v);
    }
    return 0;
}