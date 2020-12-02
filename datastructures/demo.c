// gcc -o demo demo.c
// ./demo
#include "list.h"

int main() {

    user acc;

    // Creat a new player----------------------------------------
    acc = makenode("Batman", 123);
    insertlast(&accounts, acc);

    // Insert to room and change status for player in room
    int i = insertroom(&rooms, 1, "Batman", acc);
    if(i==1) {
        printf("New room create\n");
    }
    //changestate(accounts, ROOM, "Batman");
    //printnode(findnode(accounts, "Batman")->user);
    //===========================================================
    acc = makenode("Superman", 456);
    insertlast(&accounts, acc);
    insertplayer(&rooms, 1, acc);
    
    acc = makenode("WonderWoman", 789);
    insertlast(&accounts, acc);
    insertplayer(&rooms, 1, acc);
    
    acc = makenode("GreenLantern", 101);
    insertlast(&accounts, acc);
    quickjoin(&rooms, acc);  // quick join
    
    acc = makenode("Flash", 102);
    insertlast(&accounts, acc);
    insertplayer(&rooms, 1, acc);
    

    acc = makenode("Robin", 103);
    insertlast(&accounts, acc);
    insertplayer(&rooms, 1, acc); // join when room is full
    quickjoin(&rooms, acc);  // quick join

    
    printf("--------------------------------------------\n");

    listroomptr foundPtr = findroom(rooms, 1);
    printroomplayer(foundPtr);
    //printlistuser(accounts);
    playgame(1);
    printroomplayerboard(foundPtr, "Batman");
    printroomplayerboard(foundPtr, "Superman");
    printroomplayerboard(foundPtr, "Flash");
    //printroomplayer(findroom(rooms, 2));
    //insertlast(&accounts, acc);
    

    return 0;
}