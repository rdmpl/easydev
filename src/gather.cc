#include <vector>
#include <numeric>
#include <easydev/gather.h>
Gather::Gather(int n):f(n, 0){
        iota(f.begin(), f.end(), 0);
}
int Gather::find(int v){
        return  f[v] == v? v : (f[v] = find(f[v]));
}
void Gather::merge(int a, int b){
        f[find(a)] = find(b);
}


