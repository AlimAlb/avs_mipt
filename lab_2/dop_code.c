#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long from_dopcode(long long cell){
    if (cell / 0x10000000000000 / 8  == 0){
        return cell;
    }
    else{
        long long tmp = ~(cell-1) & 0x00FFFFFFFFFFFFFF;
        tmp = -tmp;
        return tmp;
    }
}
// FF FFFF FFFF FFFF
// 0000000011111111 1111111111111111 1111111111111111 1111111111111111

int main(void){
    printf("%lld", num_to_doзcode(0x00AB01AB02AB03));
}

