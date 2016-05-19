## Synopsis

An implementation of the Nelder-Mead simplex method.
Version 1.0

## Code Example

See testfun and maxpower.

The testfun example evaluates the Rosenbrock function.

    The minimum was found at
    1.000000e+00
    1.000000e+00
    123 Function Evaluations
    61 Iterations through program

The maxpower program is an example of using Nelder-Mead for a one
dimensional optimization problem. The problem is to find the value of
a load resistor that will allow maximum power to be delivered to the
load. For this example I used a simple voltage divider circuit
consisting of a single 9V source with two resistors in series (1000
ohm and 470 ohm) with the output taken across the 470 ohm
resistor. From the Thevenin equivalent circuit we know that the
optimium load resistor value for max power transfer should be 319.73
ohms. The source code for this problem can be found here. The program
produces the following output starting with an initial guess of RL=100
ohms:

	RL:320.000000 P:0.006474
	29 Function Evaluations
	13 Iterations through program



## Motivation

The Nelder-Mead Simplex Method is direct search algorithm that's
useful for non-linear optimization problems. I was researching
optimization of antenna arrarys at one point and implemented several
versions of the Nelder-Mead algorithm.

## Installation

make

## License

Copyright (c) 1999-present Michael F. Hutt
Released under the MIT License

