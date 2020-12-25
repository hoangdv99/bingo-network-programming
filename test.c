#include<stdio.h>
int x = 0;

void func(){
    x = 1;
}

int main(int argc, char const *argv[])
{
    
        func();
        if (x == 1)
        {
            printf("OK!");
        }
        
    
    return 0;
}
