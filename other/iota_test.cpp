#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int main(){
    vector<int> ve(5);
    cout << "Size = " << ve.size() << endl;
    iota(ve.begin(), ve.end(), 0);
    cout << "Size = " << ve.size() << endl;
    EXPECT
    return 0;
}