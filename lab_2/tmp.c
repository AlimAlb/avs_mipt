#include<stdio.h>
#include <stdlib.h>
#include <math.h>

//TODO:
// run_operations
// реализовать хранение чисел в дополнительном коде

long long MEMORY[0x10000] = {0};
int used[0x10000] = {0};

// check on real numbers

long long to_number(long long num){
    if (num / 0x10000000000000 / 8  == 0){
        return num;
    }
    else{
        long long tmp = ~(num-1) & 0x00FFFFFFFFFFFFFF;
        tmp = -tmp;
        return tmp;
    }
}

void add(int addr1, int addr2, int addr3){
    printf("(addr1->%.4x, value->%lld)  +  (addr2->%.4x, value->%lld) ---> ", addr1, to_number(MEMORY[addr1]), addr2, to_number(MEMORY[addr2]));
    MEMORY[addr3] = to_number(addr1) + to_number(addr2);
    printf("result_addr->%.4x, value->%lld\n", addr3, to_number(MEMORY[addr3]));
}

void diffirence(int addr1, int addr2, int addr3){
    printf("(addr1->%.4x, value->%lld)  -  (addr2->%.4x, value->%lld) ---> ", addr1, to_number(MEMORY[addr1]), addr2, to_number(MEMORY[addr2]));
    MEMORY[addr3] = to_number(addr1) - to_number(addr2);
    printf("result_addr->%.4x, value->%lld\n", addr3, to_number(MEMORY[addr3]));
    }

void mult(int addr1, int addr2, int addr3){
    printf("(addr1->%.4x, value->%lld)  *  (addr2->%.4x, value->%lld) ---> ", addr1, to_number(MEMORY[addr1]), addr2, to_number(MEMORY[addr2]));
    MEMORY[addr3] = to_number(MEMORY[addr1]) * to_number(MEMORY[addr2]);
    printf("result_addr->%.4x, value->%lld\n", addr3, to_number(MEMORY[addr3]));
}

void division(int addr1, int addr2, int addr3){
    printf("(addr1->%.4x, value->%lld)  /  (addr2->%.4x, value->%lld) ---> ", addr1, to_number(MEMORY[addr1]), addr2, to_number(MEMORY[addr2]));
    MEMORY[addr3] = to_number(MEMORY[addr1]) / to_number(MEMORY[addr2]);
    MEMORY[addr3 + 1] = to_number(MEMORY[addr1]) % to_number(MEMORY[addr2]);
    printf("result_addr_1->%.4x, value->%lld // result_addr_2->%.4x, value->%lld\n", addr3, to_number(MEMORY[addr3]), addr3+1, to_number(MEMORY[addr3+1]));
}

long long operation(int code_of_operation,int a1,int a2,int a3){
    long long coop = code_of_operation;
    coop = coop << 48; 
    long long addr_1 = a1;
    addr_1 = addr_1 << 32;
    long long addr_2 = a2;
    addr_2 = addr_2 << 16;
    long long addr_3 = a3;
    return coop + addr_1 + addr_2 + addr_3;
}

int* decompose_operation(int addr, int* opers){
    // if (MEMORY[addr] < 0){
    //     MEMORY[addr] = ~(MEMORY[addr]) + 1;
    // }
    unsigned long long tmp = MEMORY[addr];
    for (size_t i = 0; i < sizeof(long long)*2-2; i++)
    {
        int hex_dig = tmp % 0x10;
        tmp = tmp / 0x10;

        if (i < 4){
            opers[3] += hex_dig * pow(0x10, i);
            continue;
        }
        if(i < 8){
            opers[2] += hex_dig * pow(0x10, i-4);
            continue;
        }
        if(i < 12){
            opers[1] += hex_dig * pow(0x10, i-8);
            continue;
        }
        opers[0] += hex_dig * pow(0x10, i-12);
            continue;
        
    }
    return opers;



    
}

int run_operation(int* opers,int curr_cell){
    switch (opers[0])
    {
        case 0x1: add(opers[1], opers[2], opers[3]); return curr_cell+1; 
        case 0x2: diffirence(opers[1], opers[2], opers[3]); return curr_cell+1; 
        case 0x3: mult(opers[1], opers[2], opers[3]); return curr_cell+1; 
        case 0x4: division(opers[1], opers[2], opers[3]); return curr_cell+1;

        case 0x80: return opers[3];

        case 0x81: 
            printf("if (addr1->%.4x, value->%lld) == (addr2->%.4x,value->%lld) then go to %.4x\n", opers[1], MEMORY[opers[1]], opers[2], MEMORY[opers[2]], opers[3]);
            return MEMORY[opers[1]] == MEMORY[opers[2]] ? opers[3] : curr_cell+1;
        case 0x82:
            printf("if (addr1->%.4x, value->%lld) != (addr2->%.4x,value->%lld) then go to %.4x\n", opers[1], MEMORY[opers[1]], opers[2], MEMORY[opers[2]], opers[3]);
            return MEMORY[opers[1]] != MEMORY[opers[2]] ? opers[3] : curr_cell+1;
        case 0x83:
            printf("if (addr1->%.4x, value->%lld) < (addr2->%.4x,value->%lld) then go to %.4x\n", opers[1], MEMORY[opers[1]], opers[2], MEMORY[opers[2]], opers[3]);
            return MEMORY[opers[1]] < MEMORY[opers[2]] ? opers[3] : curr_cell+1;
        case 0x84:
            printf("if (addr1->%.4x, value->%lld) >= (addr2->%.4x,value->%lld) then go to %.4x\n", opers[1], MEMORY[opers[1]], opers[2], MEMORY[opers[2]], opers[3]);
            return MEMORY[opers[1]] >= MEMORY[opers[2]] ? opers[3] : curr_cell+1;
        case 0x85:
            printf("if (addr1->%.4x, value->%lld) <= (addr2->%.4x,value->%lld) then go to %.4x\n", opers[1], MEMORY[opers[1]], opers[2], MEMORY[opers[2]], opers[3]);
            return MEMORY[opers[1]] <= MEMORY[opers[2]] ? opers[3] : curr_cell+1;
        case 0x86:
            printf("if (addr1->%.4x, value->%lld) > (addr2->%.4x,value->%lld) then go to %.4x\n", opers[1], MEMORY[opers[1]], opers[2], MEMORY[opers[2]], opers[3]);
            return MEMORY[opers[1]] < MEMORY[opers[2]] ? opers[3] : curr_cell+1;

        case 0x00: printf("(addr1->%.4x, value->%lld) := (addr2->%.4x,value->%lld)\n", opers[3], MEMORY[opers[3]], opers[1], MEMORY[opers[3]]); MEMORY[opers[3]] = MEMORY[opers[1]]; return curr_cell+1;

        case 0x99: printf("code 99 ----> end of execution"); return sizeof(MEMORY)/8;
    }
}

void exec(){ 
    int curr_cell = 0;
    do{
        int opers[4] = {0};
        printf("current memory cell --> %.4x ---- ", curr_cell);
        curr_cell = run_operation(decompose_operation(curr_cell, opers), curr_cell);
    } while (curr_cell < sizeof(MEMORY) / 8);
}

int char_to_hex(char ch){
    switch (ch)
    {
    case '0':return 0; 
    case '1':return 1; 
    case '2':return 2; 
    case '3':return 3; 
    case '4':return 4; 
    case '5':return 5; 
    case '6':return 6; 
    case '7':return 7;
    case '8':return 8;
    case '9':return 9;
    case 'A':return 10;
    case 'B':return 11;
    case 'C':return 12; 
    case 'D':return 13; 
    case 'E':return 14; 
    case 'F':return 15;
    }
}

void assemble_operations(char* str, int* info){
    for (int i = 3; i >= 0; i--)
    {
        info[0] += char_to_hex(str[i]) * pow(0x10, 3-i);
    }
    for (int i = 6; i >= 5; i--)
    {
        info[1] += char_to_hex(str[i]) * pow(0x10, 6-i);
    }
    for (int i = 11; i >= 8; i--)
    {
        info[2] += char_to_hex(str[i]) * pow(0x10, 11-i);
    }
    for (int i = 16; i >= 13; i--)
    {
        info[3] += char_to_hex(str[i]) * pow(0x10, 16-i);
    }
    for (int i = 21; i >= 18; i--)
    {
        info[4] += char_to_hex(str[i]) * pow(0x10, 21-i);
    }
    // for (size_t i = 0; i < 5; i++)
    // {
    //     printf("%x ", info[i]);
    // }
    // printf("\n");

}

void read_code(char* path){
    FILE* ptr = fopen(path, "r");
    char ch;
    char tmp[20] = {0};
    int i = 0;
    do {
        ch = fgetc(ptr);
        if (ch == '\n'){
            i = 0;
            int info[5] = {0};
            assemble_operations(tmp, info);
            MEMORY[info[0]] = operation(info[1], info[2], info[3], info[4]);
            used[info[0]] = 1;
            continue;
        }
        tmp[i++] = ch;
    } while (ch != EOF);

    fclose(ptr);
    
}

void show_memory(){
    for (int i = 0; i < sizeof(MEMORY)/8; i++)
    {   
        if (used[i] == 1) {
            int opers[4] = {0};
            printf("%.4x - ", i);
            decompose_operation(i, opers);
        
            for (size_t j = 0; j < 4; j++)
            {
                if(j == 0)
                   printf("%.2x ", opers[j]);
                else
                    printf("%.4x ", opers[j]);
            }
            printf("\n");
        }
    }
    
}

int main(int argc, char *argv[])
{
    printf("\ncode file ---> %s\n", argv[1]);
    read_code(argv[1]);
    printf("\n↓↓Memory before the execution↓↓\n\n");
    show_memory();
    printf("\n");
    printf("\n-------------start of execution-------------\n\n");
    exec();
    printf("\n\n-------------end of execution-------------\n\n");
    printf("\n↓↓Memory at the end of execution↓↓\n\n");
    show_memory();
    // MEMORY[100] = 0xfffffffffffe999b;
    // int opers[4] = {0}; 
    // decompose_operation(100, opers);
    // printf("%x - %x - %x - %x", opers[0], opers[1], opers[2], opers[3]);
    // printf("%d", sizeof(long long)*2-2);
    
    
}


// fffffffffffe999b
// fffffffffffe999b