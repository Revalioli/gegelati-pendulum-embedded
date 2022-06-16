/*
 * Bench.cpp
 *
 *  Created on: 16 juin 2022
 *      Author: emontmas
 */

#include "Bench.h"

Bench::Bench(void (*fun)(void)) : benchFunction(fun), state(BenchState::Failed) {}

void Bench::setBenchFunction(void (*fun)(void)){ this->benchFunction = fun; }
