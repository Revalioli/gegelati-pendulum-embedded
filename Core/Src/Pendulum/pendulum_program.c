/**
 * File generated with GEGELATI v1.1.0
 * On the 2022-06-03 12:58:16
 * With the CodeGen::ProgramGenerationEngine.
 */

#include "pendulum_program.h"
#include <stdint.h>
#include "externHeader.h" 	

extern double* in1;

double P0(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {5, 9, -7, 4, 10};
	return reg[0];
}

double P1(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-4, -1, -5, -6, 6};
	{
		double op0 = in1[1];
		double op1 = reg[3];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

double P2(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-3, 4, 1, 8, -5};
	{
		double op0 = in1[1];
		reg[0] = tan(op0);
	}
	return reg[0];
}

double P3(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {8, -5, 0, 2, -4};
	{
		double op0 = in1[0];
		reg[0] = exp(op0);
	}
	return reg[0];
}

double P4(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-2, 8, 4, -8, 10};
	{
		double op0 = in1[0];
		reg[0] = log(op0);
	}
	return reg[0];
}

double P5(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {6, 6, -7, 0, 7};
	{
		double op0 = reg[6];
		double op1 = in1[1];
		reg[0] = ((op0) < (op1)) ? (op1) : (op0);
	}
	return reg[0];
}

double P6(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-9, -8, 4, -8, 10};
	{
		double op0 = reg[5];
		double op1 = in1[1];
		reg[4] = op0 / op1;
	}
	{
		double op0 = reg[4];
		reg[0] = log(op0);
	}
	return reg[0];
}

double P7(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {5, 9, -7, 4, -3};
	{
		double op0 = reg[0];
		double op1 = in1[0];
		reg[0] = ((op0) < (op1)) ? (op1) : (op0);
	}
	return reg[0];
}

double P8(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {8, -5, 0, 2, -4};
	{
		double op0 = in1[0];
		reg[0] = exp(op0);
	}
	{
		double op0 = reg[0];
		double op1 = in1[0];
		reg[5] = ((op0) < (op1)) ? (op1) : (op0);
	}
	{
		double op0 = reg[5];
		double op1 = in1[1];
		reg[0] = op0 / op1;
	}
	return reg[0];
}

double P9(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-2, -8, 4, -8, 10};
	{
		double op0 = reg[5];
		double op1 = in1[1];
		reg[4] = op0 / op1;
	}
	{
		double op0 = reg[4];
		reg[0] = log(op0);
	}
	{
		double op0 = reg[0];
		double op1 = reg[5];
		reg[0] = ((op0) < (op1)) ? (op1) : (op0);
	}
	return reg[0];
}

double P10(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {8, 5, 3, -10, -4};
	{
		double op0 = in1[0];
		reg[4] = exp(op0);
	}
	{
		double op0 = reg[4];
		reg[3] = exp(op0);
	}
	{
		double op0 = reg[3];
		reg[0] = cos(op0);
	}
	return reg[0];
}

double P11(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-9, 5, 0, -10, -1};
	{
		double op0 = in1[1];
		reg[0] = tan(op0);
	}
	return reg[0];
}

double P12(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {8, -5, 0, 2, -4};
	{
		double op0 = reg[0];
		double op1 = in1[1];
		reg[4] = op0 - op1;
	}
	{
		double op0 = in1[0];
		double op1 = reg[4];
		reg[4] = op0 - op1;
	}
	{
		double op0 = reg[4];
		double op1 = reg[4];
		reg[0] = op0 - op1;
	}
	{
		double op0 = reg[0];
		double op1 = in1[0];
		reg[5] = ((op0) < (op1)) ? (op1) : (op0);
	}
	{
		double op0 = reg[5];
		double op1 = in1[1];
		reg[0] = op0 / op1;
	}
	return reg[0];
}

double P13(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-5, 7, 8, -5, -2};
	{
		double op0 = in1[1];
		int32_t op1 = cst[4];
		reg[1] = op0 * ((double)(op1) / 10.0);
	}
	{
		double op0 = reg[1];
		reg[0] = tan(op0);
	}
	return reg[0];
}

double P14(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-9, -9, -10, 10, 10};
	{
		double op0 = reg[3];
		reg[5] = exp(op0);
	}
	{
		double op0 = reg[5];
		double op1 = in1[1];
		reg[0] = op0 / op1;
	}
	return reg[0];
}

double P15(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {2, 7, 5, -7, -2};
	{
		double op0 = reg[6];
		double op1 = in1[0];
		reg[0] = ((op0) < (op1)) ? (op1) : (op0);
	}
	return reg[0];
}

double P16(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-4, -1, 4, -6, 6};
	{
		double op0 = in1[1];
		reg[0] = M_PI;
	}
	return reg[0];
}

double P17(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {5, 6, 7, -10, -7};
	{
		double op0 = in1[0];
		reg[3] = cos(op0);
	}
	{
		double op0 = reg[3];
		reg[0] = tan(op0);
	}
	return reg[0];
}

double P18(){
	// double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	double reg[8];
	for(int i = 0; i < 8; i++)
		reg[i] = 0.0;

	int32_t cst[5] = {6, 6, 9, 2, 7};
	{
		double op0 = in1[0];
		double op1 = in1[1];
		reg[0] = ((op0) < (op1)) ? (op1) : (op0);
	}
	return reg[0];
}

double P19(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-9, 5, 5, 7, -3};
	{
		double op0 = in1[1];
		int32_t op1 = cst[4];
		reg[6] = op0 * ((double)(op1) / 10.0);
	}
	{
		double op0 = reg[6];
		double op1 = reg[4];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P20(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {6, -7, -7, 0, 7};
	{
		double op0 = reg[4];
		reg[4] = log(op0);
	}
	{
		double op0 = reg[4];
		reg[0] = tan(op0);
	}
	return reg[0];
}

double P21(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {1, 6, -6, -2, 8};
	{
		double op0 = reg[0];
		double op1 = in1[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

double P22(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-6, 8, 6, -10, 1};
	{
		double op0 = in1[0];
		reg[0] = exp(op0);
	}
	return reg[0];
}

double P23(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {6, 8, 0, -10, 3};
	{
		double op0 = in1[1];
		reg[0] = log(op0);
	}
	return reg[0];
}

double P24(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {9, 8, 6, -10, 1};
	{
		double op0 = reg[0];
		reg[0] = exp(op0);
	}
	return reg[0];
}

double P25(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {8, 8, -10, 2, -4};
	{
		double op0 = in1[1];
		double op1 = in1[1];
		reg[0] = op0 - op1;
	}
	{
		double op0 = reg[0];
		double op1 = in1[0];
		reg[5] = ((op0) < (op1)) ? (op1) : (op0);
	}
	{
		double op0 = reg[5];
		double op1 = in1[1];
		reg[0] = op0 / op1;
	}
	return reg[0];
}

double P26(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-6, -5, -8, 10, 1};
	{
		double op0 = in1[1];
		reg[7] = tan(op0);
	}
	{
		double op0 = reg[7];
		reg[6] = log(op0);
	}
	{
		double op0 = in1[1];
		reg[3] = log(op0);
	}
	{
		double op0 = reg[3];
		double op1 = reg[6];
		reg[0] = ((op0) < (op1)) ? (op1) : (op0);
	}
	return reg[0];
}

double P27(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {3, 10, 6, -10, 7};
	{
		double op0 = in1[0];
		double op1 = reg[4];
		reg[3] = op0 + op1;
	}
	{
		double op0 = reg[3];
		reg[2] = sin(op0);
	}
	{
		double op0 = reg[2];
		double op1 = in1[1];
		reg[0] = op0 * op1;
	}
	return reg[0];
}

double P28(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {4, 8, 7, -10, -10};
	return reg[0];
}

double P29(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {10, -4, -4, -10, 6};
	{
		double op0 = reg[4];
		reg[0] = log(op0);
	}
	return reg[0];
}

double P30(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {8, 4, 10, 8, -5};
	{
		double op0 = reg[2];
		double op1 = in1[1];
		reg[2] = ((op0) < (op1)) ? (op1) : (op0);
	}
	{
		double op0 = reg[2];
		reg[0] = exp(op0);
	}
	return reg[0];
}

double P31(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-5, -1, 10, 2, -6};
	{
		double op0 = reg[0];
		reg[5] = M_PI;
	}
	{
		double op0 = in1[0];
		double op1 = reg[5];
		reg[4] = op0 + op1;
	}
	{
		double op0 = reg[4];
		int32_t op1 = cst[4];
		reg[0] = op0 * ((double)(op1) / 10.0);
	}
	return reg[0];
}

double P32(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {0, 8, 10, -9, -8};
	{
		double op0 = reg[0];
		reg[0] = log(op0);
	}
	return reg[0];
}

double P33(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {1, -6, 10, -7, -2};
	{
		double op0 = reg[6];
		reg[2] = sin(op0);
	}
	{
		double op0 = reg[2];
		reg[0] = exp(op0);
	}
	return reg[0];
}

double P34(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {5, 6, 6, -10, -10};
	{
		double op0 = reg[4];
		reg[5] = sin(op0);
	}
	{
		double op0 = in1[0];
		double op1 = reg[5];
		reg[4] = ((op0) < (op1)) ? (op1) : (op0);
	}
	{
		double op0 = reg[4];
		reg[0] = log(op0);
	}
	return reg[0];
}

double P35(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {5, -1, -3, -2, -6};
	{
		double op0 = reg[0];
		double op1 = reg[0];
		reg[3] = op0 + op1;
	}
	{
		double op0 = in1[0];
		double op1 = reg[3];
		reg[4] = op0 + op1;
	}
	{
		double op0 = reg[4];
		int32_t op1 = cst[2];
		reg[0] = op0 * ((double)(op1) / 10.0);
	}
	return reg[0];
}

double P36(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {1, -2, 10, -3, -10};
	{
		double op0 = in1[0];
		reg[0] = exp(op0);
	}
	{
		double op0 = reg[0];
		reg[2] = sin(op0);
	}
	{
		double op0 = reg[2];
		double op1 = in1[1];
		reg[2] = op0 - op1;
	}
	{
		double op0 = reg[2];
		double op1 = in1[1];
		reg[2] = ((op0) < (op1)) ? (op1) : (op0);
	}
	{
		double op0 = reg[2];
		reg[0] = exp(op0);
	}
	return reg[0];
}

double P37(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {-7, 4, 6, -10, 9};
	{
		double op0 = in1[1];
		reg[0] = cos(op0);
	}
	{
		double op0 = reg[0];
		reg[0] = exp(op0);
	}
	return reg[0];
}

double P38(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int32_t cst[5] = {4, 6, -7, 2, -9};
	{
		double op0 = in1[1];
		double op1 = in1[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}
