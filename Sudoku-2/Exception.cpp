#include "Exception.h"

NumberException::NumberException() :exception("数独题目个数number应在1~10000范围内")
{
}

ModeException::ModeException() : exception("数独难度mode应在1~3范围内")
{
}

RangeException::RangeException() : exception("挖空个数下限lower与上限upper应满足0<=lower<=upper<=55")
{
}

ValidException::ValidException() : exception("非法数独题目无法求解")
{
}

