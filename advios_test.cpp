#include "advios.h"
#include <iostream>


SC_MODULE(testbench_advios) {
	SC_CTOR(testbench_advios): s_clk("clk", 8,SC_NS)
	{
		advios = new advios("advios");
		tracefile = sc_create_vcd_trace_file("test_setup");

		SC_CTHREAD(test_setup, advios->clk.pos());

		advios->clk(s_clk);
		advios->reset(s_reset);
		advios->inSwitch(s_inSwitch);
		advios->outLeds(s_outLeds);
		advios->ctrl(s_ctrl);
	}

	~testbench_advios()
	{
		delete advios;
	}


	void test_setup() {
		// Testing reset
		// Set variables
		s_ctrl = 0;
		s_inSwitch = 0;
		s_reset = false;
		wait(40, SC_NS); // wait for 6 clk cycles
		s_reset = true;
		wait(16, SC_NS); // wait for 2 clk cycles
		s_reset = false;
		wait(); // Wait one clk cycle for the values to be changed
		std::cout << "Test button reset is " <<
				((s_outLeds.read() == 0) ? "success" : "failure" );
		std::cout << ", the value of s_outleds is " << s_outLeds.read()
				<< " and the expected value is 0.\n";


		// Testing ctrl = 0 and inSwitch = 8, outLeds must reset
		wait(24, SC_NS);
		s_inSwitch = 8;
		wait();
		s_inSwitch = 0;
		wait(); // Wait two clk cycles (simulation of one second is 2 clk cycles)
		// for the outLeds to be incremented
		std::cout << "Test of ctrl=0 and inSwitch=8 is " <<
				((s_outLeds.read() == 0) ? "success" : "failure" );
		std::cout << ", the value of s_outleds is " << s_outLeds.read()
				<< " and the expected value is 0.\n";


		// Testing ctrl=6, inSwitch=5, outLeds = 6 AND 5 = 4
		wait(24, SC_NS);
		s_ctrl = 6;
		s_inSwitch = 7;
		wait();
		s_ctrl = 0;
		wait();
		std::cout << "Test of ctrl=6 and inSwitch=7 is " <<
						((s_outLeds.read() == 6) ? "success" : "failure" );
				std::cout << ", the value of s_outleds is " << s_outLeds.read()
						<< " and the expected value is 6.\n";
	}

	void start_tracefile() {
		sc_trace(tracefile, s_reset, "reset");
		sc_trace(tracefile, s_clk, "clk");
		sc_trace(tracefile, s_inSwitch, "inSwitch");
		sc_trace(tracefile, s_outLeds, "outLeds");
		sc_trace(tracefile, s_ctrl, "ctrl");
	}

	void close_tracefile() {
		sc_close_vcd_trace_file(tracefile);
	}

	// Variables
	sc_trace_file* tracefile;
	advios* advios;

	// Ports and signals
	sc_clock s_clk;
	sc_signal<bool> s_reset;
	sc_signal<sc_uint<NUM_BITS> > s_inSwitch;
	sc_signal<sc_uint<NUM_BITS> > s_outLeds;
	sc_signal<sc_uint<NUM_BITS> > s_ctrl;
};

int sc_main(int argc, char* argv[]) {
	sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
	sc_report_handler::set_actions(SC_ID_LOGIC_X_TO_BOOL_, SC_LOG);
	sc_report_handler::set_actions(SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_LOG);
	sc_report_handler::set_actions(SC_ID_OBJECT_EXISTS_, SC_LOG);

	testbench_advios uut("advios_test");

	uut.start_tracefile();

	sc_start(200, SC_NS);

	uut.close_tracefile();

	return 0;
}
