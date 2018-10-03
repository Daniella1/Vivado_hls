#include "advios.h"

void advios::controlLeds() {
	static sc_uint<4> count;
	count = 0;
	outLeds = 0;

	while(true)
	{
		wait();
		// If ctrl is 0, then increment the outLeds value every second
		if (ctrl.read() == 0)
		{
			if (one_second_passed_in.read() == true && inSwitch.read() != 8)
				outLeds = ++count;

			if (inSwitch.read() == 8)
				outLeds = 0;
		}

		// If ctrl is a value between 1 and 15, the the outLeds is set to ctrl AND inSwitch
		else if (ctrl.read() > 0 && ctrl.read() < 16)
			outLeds = ctrl.read() & inSwitch.read();

	}
}

void advios::pulseCounter() {
#ifdef __SYNTHESIS__
	static const int ticks_pr_increment = 125000000; // 8 nanoseconds * ticks_pr_increment = 1 second
#else
	static const int ticks_pr_increment = 2;
#endif
	static sc_uint<27> clk_counter = 1;
	// the counter is set to one, since we wait a clk cycle in the loop, and we compensate for this

	one_second_passed_out = false;

	while (true) {
		wait();
		one_second_passed_out = false;
		++clk_counter;

		if (clk_counter >= ticks_pr_increment) {
			one_second_passed_out = true;
			clk_counter = 0;
		}

	}
}
