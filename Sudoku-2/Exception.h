#pragma once

#include <iostream>

using namespace std;

class NumberException :public exception
{
public:
    NumberException();
};

class ModeException :public exception
{
public:
    ModeException();
};

class RangeException :public exception
{
public:
    RangeException();
};

class ValidException :public exception
{
public:
    ValidException();
};


