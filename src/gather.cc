#include <vector>
#include <numeric>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <easydev/gather.h>
Gather::Gather(int n):f(n, 0){
        srand((unsigned int)time(0));       
        iota(f.begin(), f.end(), 0);
}
int Gather::find(int v){
        return  f[v] == v? v : (f[v] = find(f[v]));
}
void Gather::merge(int a, int b){
        if(a == b){
            return;
        }
        if(rand() & 1){
            std::swap(a, b);
        }
        f[find(a)] = find(b);
}


