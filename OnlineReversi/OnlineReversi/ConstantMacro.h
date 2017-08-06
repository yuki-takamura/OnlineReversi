#pragma once

#define HORIZONTAL 8
#define VERTICAL 8

#define COLOR_BLACK 0
#define COLOR_WHITE 7

#define MAX_STRING_NUM 3
#define EMPTY_NUM -1
#define VECTOR_NUM 8 //判定が必要な周囲マスの数

#define EXIT_STRING "000"

///<summary>
///ある地点からX軸上で離れている値
///</summary>
const int VECTOR_HORIZONTAL[] =
{
	-1, 0, 1,
	-1,    1,
	-1, 0, 1 
};

///<summary>
///ある地点からY軸上で離れている値
///</summary>
const int VECTOR_VERTICAL[] =
{
	-1,-1,-1,
	 0,    0,
	 1, 1, 1
};