/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

namespace {
#define PERIOD_MS 2000ms
}

int main()
{
	while (true) {
		printf("Alive! \n");
		ThisThread::sleep_for(PERIOD_MS / 2);
	}
}
