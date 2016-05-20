## Synopsis

An implementation of the Nelder-Mead simplex method.  
Version 1.1

## Code Example

See rosenbrock and maxpower.

The rosenbrock example finds the minimum for the Rosenbrock function.

In the first case a starting point of (-1.2,1.0) was specified.

![alt text](http://www.mikehutt.com/images/simplex.png "Initial Simplex")

    Initial Values
    -1.20, 1.00, value 24.20
    -0.23, 1.26, value 147.22
    -0.94, 1.97, value 121.79
    123 Function Evaluations
    61 Iterations through program
    The minimum was found at
    1.00, 1.00, value 0.00

In the second case no starting point was specified.

    Initial Values
    0.00, 0.00, value 1.00
    0.97, 0.26, value 46.36
    0.26, 0.97, value 81.98
    102 Function Evaluations
    50 Iterations through program
    The minimum was found at
    1.00, 1.00, value 0.00

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

	Initial Values
	100.000000, value -0.004700
	101.000000, value -0.004725
	29 Function Evaluations
	13 Iterations through program
	320.00 ohms, 0.006474 watts


## Motivation

The Nelder-Mead Simplex Method is a direct search algorithm that's
useful for non-linear optimization problems. I was researching
optimization of antenna arrarys at one point and implemented several
versions of the Nelder-Mead algorithm.

Reference for the creation of the initial simplex.  
D. J. Wilde and C. S. Beightler, Foundations of Optimization. Englewood Cliffs, N.J., Prentice-Hall, 1967, p. 319

The pn,qn values used to create the initial simplex are defined by
(Spendly, Hext, and Himsworth). This creates a simplex with unit edges
between all vertices. Some implementations just form the initial
simplex by taking a step in each direction. Since we're talking about
non-linear optimization, there is no one initial simplex that is best
for every case. For my applications, concerning antenna array
optimization, the initial simplex with unit edges seems to give the
best results. Although it does have to be scaled at times depending on
the problem.

## Installation

make

## License

Copyright (c) 1997 Michael F. Hutt, Released under the MIT License

