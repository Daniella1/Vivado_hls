#include "systemc.h"

#ifndef ADVIOS_H
#define ADVIOS_H

#define NUM_BITS 4

SC_MODULE(advios) {
	SC_CTOR(advios)
	{
		SC_CTHREAD(controlLeds, clk.pos());
		reset_signal_is(reset, true);

		SC_CTHREAD(pulseCounter, clk.pos());
		reset_signal_is(reset, true);

		this->one_second_passed_in(one_second_passed_sig);
		this->one_second_passed_out(one_second_passed_sig);
	}

	void controlLeds();
	void pulseCounter();

	// Define objects and variables
	sc_in<bool> clk;
	sc_in<bool> reset;
	sc_in<sc_uint<NUM_BITS> > ctrl;
	sc_in<sc_uint<NUM_BITS> > inSwitch;
	sc_out<sc_uint<NUM_BITS> > outLeds;

	// Signal between the two threads
	sc_signal<bool> one_second_passed_sig;
	sc_out<bool> one_second_passed_out;
	sc_in<bool> one_second_passed_in;

};

#endif
