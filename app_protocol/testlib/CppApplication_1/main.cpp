/*
 * File:   main.c
 * Author: Leonardo Berti
 *
 * Created on 7 ottobre 2012, 14.17
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "sbuilder.h"


#define MAX_SZ 256

static char buff[MAX_SZ];
static char buff2[MAX_SZ];

using namespace std;

SBuilder builder(buff,MAX_SZ);

SBuilder builder2(buff2,MAX_SZ);

void display(){

    cout<<builder.getBuffer()<<"    (len="<<builder.getLength()<<")"<<endl;
}

void test1()
{
    builder.append("prova",strlen("prova"));

    display();

    builder.append('A');

    display();

    builder.append("12345");

    display();

    builder.setLength(5);

    display();

//    builder.append(122);
//
//    display();

    builder.append(" valore=");

    builder.append(122);

    builder.setLength(0);

    builder.append(0.009);

    display();

    builder.append("   v2=");

    builder.append(189.292);

    display();

    builder2.append("testo del builder 2!!:)");

    builder.append(builder2);
    
    display();

}


/*
 *
 */
int main(int argc, char** argv) {
    
    test1();

    return (EXIT_SUCCESS);
}




