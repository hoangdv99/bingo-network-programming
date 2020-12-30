#include<stdio.h>
#include<string.h>

int x = 0;

void func(){
    x = 1;
}

int main(int argc, char const *argv[])
{
    char boardString[100];
    char num[5];

    for (int i = 0; i < 2; i++)
    {
        strcpy(boardString, "");
            for (int n = 0; n < 5; n++)
            {
                sprintf(num, "%d", 1);
                strcat(boardString, "@");
                strcat(boardString, num);
            }
            strcat(boardString, " ");
        
        // strcpy(res->data, boardString);
        // setMessageResponse(res);
        // sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
        printf("%s\n", boardString);
        printf("-------------------------");
    }
    
}
