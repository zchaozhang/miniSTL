#include <iostream>
#include <vector>
#include "miniSTL_priority_queue.h"
#include <algorithm>
#include "miniSTL_set.h"
using namespace std;
int main() {
    zzc::set<int> i;
    i.insert(5);
    i.insert(4);
    i.insert(6);
    i.insert(10);
    auto it = i.find(4);
    cout << i.size() << endl;      //cout 4
    cout << *it << endl;           //cout 4
}