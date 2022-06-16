/**
 * File generated with GEGELATI v1.1.0
 * On the 2022-06-03 12:58:16
 * With the CodeGen::TPGGenerationEngine.
 */

#include "pendulum.h"
#include "pendulum_program.h"
#include <limits.h>
#include <assert.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

int bestProgram(double *results, int nb) {
	int bestProgram = 0;
	double bestScore = (isnan(results[0]))? -INFINITY : results[0];
	for (int i = 1; i < nb; i++) {
		double challengerScore = (isnan(results[i]))? -INFINITY : results[i];
		if (challengerScore >= bestScore) {
			bestProgram = i;
			bestScore = challengerScore;
		}
	}
	return bestProgram;
}

enum vertices {T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, A12, A13, A14, A15, A16, A17, A18, A19, };

int inferenceTPG() {
	bool teamsVisited[20] = { false };

	double T0Scores[3];
	double T1Scores[3];
	double T2Scores[4];
	double T3Scores[5];
	double T4Scores[5];
	double T5Scores[5];
	double T6Scores[5];
	double T7Scores[4];
	double T8Scores[5];
	double T9Scores[4];
	double T10Scores[5];
	double T11Scores[5];

	enum vertices currentVertex = T11;
	while(1) {
		switch (currentVertex) {
		case T0: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T0Scores[0] = P0();
				T0Scores[1] = P1();
				T0Scores[2] = P2();
			}
			int best = bestProgram(T0Scores, 3);
			T0Scores[best] = DBL_MIN;
			const enum vertices next[3] = { A12, A13, A14,  };
			currentVertex = next[best];
			break;
		}
		case T1: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T1Scores[0] = P3();
				T1Scores[1] = P4();
				T1Scores[2] = P5();
			}
			int best = bestProgram(T1Scores, 3);
			T1Scores[best] = DBL_MIN;
			const enum vertices next[3] = { A15, A16, A17,  };
			currentVertex = next[best];
			break;
		}
		case T2: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T2Scores[0] = P5();
				T2Scores[1] = P6();
				T2Scores[2] = P7();
				T2Scores[3] = P8();
			}
			int best = bestProgram(T2Scores, 4);
			T2Scores[best] = DBL_MIN;
			const enum vertices next[4] = { A17, A16, A12, A15,  };
			currentVertex = next[best];
			break;
		}
		case T3: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T3Scores[0] = P5();
				T3Scores[1] = P9();
				T3Scores[2] = P10();
				T3Scores[3] = P11();
				T3Scores[4] = P8();
			}
			int best = bestProgram(T3Scores, 5);
			T3Scores[best] = DBL_MIN;
			const enum vertices next[5] = { A17, A16, A12, A16, A15,  };
			currentVertex = next[best];
			break;
		}
		case T4: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T4Scores[0] = P5();
				T4Scores[1] = P9();
				T4Scores[2] = P10();
				T4Scores[3] = P12();
				T4Scores[4] = P8();
			}
			int best = bestProgram(T4Scores, 5);
			T4Scores[best] = DBL_MIN;
			const enum vertices next[5] = { A17, A16, A12, T3, A15,  };
			currentVertex = next[best];
			break;
		}
		case T5: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T5Scores[0] = P13();
				T5Scores[1] = P14();
				T5Scores[2] = P15();
				T5Scores[3] = P16();
				T5Scores[4] = P17();
			}
			int best = bestProgram(T5Scores, 5);
			T5Scores[best] = DBL_MIN;
			const enum vertices next[5] = { T0, A15, T2, T2, A17,  };
			currentVertex = next[best];
			break;
		}
		case T6: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T6Scores[0] = P11();
				T6Scores[1] = P18();
				T6Scores[2] = P19();
				T6Scores[3] = P20();
				T6Scores[4] = P21();
			}
			int best = bestProgram(T6Scores, 5);
			T6Scores[best] = DBL_MIN;
			const enum vertices next[5] = { A16, T4, A17, A17, A17,  };
			currentVertex = next[best];
			break;
		}
		case T7: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T7Scores[0] = P22();
				T7Scores[1] = P5();
				T7Scores[2] = P10();
				T7Scores[3] = P23();
			}
			int best = bestProgram(T7Scores, 4);
			T7Scores[best] = DBL_MIN;
			const enum vertices next[4] = { T6, A17, A12, A17,  };
			currentVertex = next[best];
			break;
		}
		case T8: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T8Scores[0] = P24();
				T8Scores[1] = P25();
				T8Scores[2] = P4();
				T8Scores[3] = P26();
				T8Scores[4] = P27();
			}
			int best = bestProgram(T8Scores, 5);
			T8Scores[best] = DBL_MIN;
			const enum vertices next[5] = { T6, A15, A16, T1, T5,  };
			currentVertex = next[best];
			break;
		}
		case T9: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T9Scores[0] = P4();
				T9Scores[1] = P28();
				T9Scores[2] = P29();
				T9Scores[3] = P30();
			}
			int best = bestProgram(T9Scores, 4);
			T9Scores[best] = DBL_MIN;
			const enum vertices next[4] = { A16, A12, A18, T8,  };
			currentVertex = next[best];
			break;
		}
		case T10: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T10Scores[0] = P31();
				T10Scores[1] = P32();
				T10Scores[2] = P33();
				T10Scores[3] = P4();
				T10Scores[4] = P34();
			}
			int best = bestProgram(T10Scores, 5);
			T10Scores[best] = DBL_MIN;
			const enum vertices next[5] = { A19, T9, T7, A16, A16,  };
			currentVertex = next[best];
			break;
		}
		case T11: {
			if (!teamsVisited[currentVertex]) {
				teamsVisited[currentVertex] = true;
				T11Scores[0] = P35();
				T11Scores[1] = P36();
				T11Scores[2] = P37();
				T11Scores[3] = P29();
				T11Scores[4] = P38();
			}
			int best = bestProgram(T11Scores, 5);
			T11Scores[best] = DBL_MIN;
			const enum vertices next[5] = { A19, T10, T6, A18, A17,  };
			currentVertex = next[best];
			break;
		}
		case A12: {
			return 10;
			break;
		}
		case A13: {
			return 11;
			break;
		}
		case A14: {
			return 1;
			break;
		}
		case A15: {
			return 0;
			break;
		}
		case A16: {
			return 12;
			break;
		}
		case A17: {
			return 6;
			break;
		}
		case A18: {
			return 4;
			break;
		}
		case A19: {
			return 2;
			break;
		}
		}
	}
}
