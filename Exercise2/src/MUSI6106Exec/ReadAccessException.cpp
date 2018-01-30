//
//  ReadAccessException.cpp
//  MUSI6106Exec
//
//  Created by zhao yan on 1/29/18.
//
#include <iostream>
#include <exception>
using namespace std;

class ReadAccessException: public exception
{
public:
    ReadAccessException();
    virtual const char* trace() const throw()
    {
        return "accessing samples haven't been writen";
    }
};
