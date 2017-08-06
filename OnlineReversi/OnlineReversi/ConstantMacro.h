#pragma once

#define HORIZONTAL 8
#define VERTICAL 8

#define COLOR_BLACK 0
#define COLOR_WHITE 7

#define MAX_STRING_NUM 3
#define EMPTY_NUM -1
#define VECTOR_NUM 8 //���肪�K�v�Ȏ��̓}�X�̐�

#define EXIT_STRING "000"

///<summary>
///����n�_����X����ŗ���Ă���l
///</summary>
const int VECTOR_HORIZONTAL[] =
{
	-1, 0, 1,
	-1,    1,
	-1, 0, 1 
};

///<summary>
///����n�_����Y����ŗ���Ă���l
///</summary>
const int VECTOR_VERTICAL[] =
{
	-1,-1,-1,
	 0,    0,
	 1, 1, 1
};