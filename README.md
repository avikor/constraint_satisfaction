
## Basic Definitions
A _Constraint Satisfaction Problem (CSP)_ is a triplet _(X, D, C)_ where _X_ is a set of variables X<sub>1</sub>, ...,X<sub>n</sub>.  
_D_ is a set of domains D<sub>1</sub>, ...,D<sub>n</sub>, where each variable X<sub>i</sub> is assigned with values from domain D<sub>i</sub>.  
And _C_ is a set of constraints C<sub>1</sub>(S<sub>1</sub>), ..., C<sub>m</sub>(S<sub>m</sub>), where each S<sub>i</sub> is a set of variables on which C<sub>i</sub> defines a constraint.  
A state of _Constraint Satisfaction Problem_ instance is defined by an **assignment** of values to some or all of its variables.  
A **complete assignment** is a state in which every variable is assigned with a value from its domain.  
A **partial assignment** is a state in which some variables are assigned and some are not.  
A **consistent assignment** is a state in which the assigned variables do not violate any constraints.  
A **complete consistent assignment** is a solution to a _Constraint Satisfaction Problem_ instance.  
A **constraint graph** (could be a hypergraph) is a graph in which the nodes correspond to varaibles  
and the edges correspond to constraints, i.e. V = X and E = C.
<br></br>

## Implemented Algorithms List
#### Solvers
1. backtracking search (with or without forward checking). Could be used to find a single solution or all solutions.
2. heuristic backtracking search: defaults to Minimum Remaining Values for choosing next unassigned variable,  
with Degree heuristic as tie breaker. Defaults to Least Constraining Value for domain sorting of chosen unassigned variable.  
Allows users to define, pick and choose custom heuristics. Can be used with or without forward checking.  
Could be used to find a single solution or all solutions.
3. min conflicts (with or without tabu search).
4. constraints weighting.
5. tree csp solver: an algorithm that can solve tree-structured constraint satisfaction problems.
6. cycle cutset: a naive cutset conditioning solver. See source code for exhaustive description.
7. simulated annealing.
8. random-restart first-choice hill climbing.
9. genetic local search.
<br></br>

#### preprocessing
1. Arc Consistency 3 (AC3). Could be given as an argument to both backtracking algorithms and thus implement  
Maintaining Arc Consistency (MAC).
2. Arc Consistency 4 (AC4).
3. Path Consistency 2 (PC2).
4. i-consistency.

All examples written with comments can be find at cspExamples.
</br>

## Example #1: Pythagorean Triples
<img src="https://freedocs.mi.hdm-stuttgart.de/Sd1/Ref/Statements/phythagorean.svg" width=200 height=200/>
  
Problem: find all triples (x, y, z) such that x<sup>2</sup> + y<sup>2</sup> = z<sup>2</sup> and x < y < z and max{x, y, z} &lt; 21.  
Variables: x, y, z &isin; {1, 2, 3, ..., 20}  
Domains: each variable's domain is (1, 2, 3, ..., 20)  
Constraints:  
1. x<sup>2</sup> + y<sup>2</sup> == z<sup>2</sup>  
2. x < y < z  

Code implementation:
```cpp
#include <csp.h>

int main()
{
    std::unordered_set<unsigned int> domain;
    domain.reserve(20);
    for (unsigned int i = 1; i < 21; ++i)
    {
        domain.insert(i);
    }

    csp::Variable<unsigned int> x{ domain };
    csp::Variable<unsigned int> y{ domain };
    csp::Variable<unsigned int> z{ domain };

    csp::Constraint<unsigned int> pythagoreanTripleConstraint{ { x, y, z },
        [](const std::vector<unsigned int>& assignedValues) -> bool
        {
            if (assignedValues.size() < 3)
            {
                return true;
            }
            unsigned int xVal = assignedValues.at(0);
            unsigned int yVal = assignedValues.at(1);
            unsigned int zVal = assignedValues.at(2);
            return xVal * xVal + yVal * yVal == zVal * zVal;
        } 
    };

    csp::Constraint<unsigned int> totalOrderConstraint{ { x, y, z },
        [](const std::vector<unsigned int>& assignedValues) -> bool
        {
            if (assignedValues.size() < 3)
            {
                return true;
            }
            return assignedValues.at(0) < assignedValues.at(1) && assignedValues.at(1) < assignedValues.at(2);
        }
    };

    csp::ConstraintProblem<unsigned int> pythagoreanTriplesProblem{ { pythagoreanTripleConstraint, totalOrderConstraint } };
    
    const std::unordered_set<csp::Assignment<unsigned int>> sols =
		csp::heuristicBacktrackingSolver_findAllSolutions<unsigned int>(
			pythagoreanTriplesProblem,
			csp::minimumRemainingValues_primarySelector<unsigned int>,
			csp::degreeHeuristic_secondarySelector<unsigned int>,
			csp::leastConstrainingValue<unsigned int>);

	for (const csp::Assignment<unsigned int>& assignment : sols)
	{
		pythagoreanTriplesProblem.unassignAllVariables();
		pythagoreanTriplesProblem.assignFromAssignment(assignment);
		pythagoreanTriplesProblem.writeNameToAssignment(std::cout);
		std::cout << '\n';
	}
        /*
	x : 3
	y : 4
	z : 5

	x : 9
	y : 12
	z : 15

	x : 5
	y : 12
	z : 13

	x : 6
	y : 8
	z : 10

	x : 8
	y : 15
	z : 17

	x : 12
	y : 16
	z : 20
	*/
}
```
## Example #2: Magic Square
![](https://upload.wikimedia.org/wikipedia/commons/e/e4/Magicsquareexample.svg)  

Problem: fill up an n x n square with distinct positive integers in the range (1, ..., n x n) such that each cell  
contains a different integer and the sum of the integers in each row, column, and diagonal is equal.  
-- Define **magic sum** to be n * ((n * n + 1) / 2).  

Variables: squares on the board.  
Domains: each variable's domain is (1, ..., n x n).   
Constraints:  
1. Each variable must be have a unique value.
2. The values of all rows sum up to **magic sum**.
3. The values of all columns sum up to **magic sum**.
4. The values of both diagonals sum up to **magic sum**.

Code implementation:
```cpp
#include <csp.h>

int main()
{
    unsigned int n = 3;
    unsigned int order = n * n;
    unsigned int magicSum = n * ((order + 1) / 2);
    
    std::unordered_set<unsigned int> domain;
    domain.reserve(order);
    for (unsigned int i = 1; i <= order; ++i)
    {
        domain.emplace(i);
    }
    
    std::vector<csp::Variable<unsigned int>> magicSquareVars;
    std::vector<csp::Constraint<unsigned int>> magicSquareConstrs;
    
    std::unordered_map<unsigned int, std::reference_wrapper<csp::Variable<unsigned int>>> squareToVarRefMap;
    std::unordered_map<std::string, std::reference_wrapper<csp::Variable<unsigned int>>> nameToVarRefMap;

    magicSquareVars.reserve(order);
    for (unsigned int i = 1; i <= order; ++i)
    {
        magicSquareVars.emplace_back(domain);
        squareToVarRefMap.emplace(i, magicSquareVars.back());
        nameToVarRefMap.emplace(std::to_string(i), variables.back());
    }
    
    magicSquareConstrs.reserve(order);
    std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> varRefs;
    varRefs.reserve(order);
    for (csp::Variable<unsigned int>& var : magicSquareVars)
    {
        varRefs.emplace_back(var);
    }
    magicSquareConstrs.emplace_back(varRefs, csp::allDiff<unsigned int>);
    
    csp::ExactLengthExactSum<unsigned int> exactMagicSumConstrEvaluator{ n, magicSum };
    
    for (unsigned int row = 1; row <= order; row += n)
    {
        std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> varRefs;
        varRefs.reserve(n);
        for (unsigned int i = row; i < row + n; ++i)
        {
            varRefs.emplace_back(squareToVarRefMap.at(i));
        }
        magicSquareConstrs.emplace_back(varRefs, exactMagicSumConstrEvaluator);
    }
    
    for (unsigned int col = 1; col <= n; ++col)
    {
        std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> varRefs;
        varRefs.reserve(n);
        for (unsigned int i = col; i <= order; i += n)
        {
            varRefs.emplace_back(squareToVarRefMap.at(i));
        }
        magicSquareConstrs.emplace_back(varRefs, exactMagicSumConstrEvaluator);
    }
    
    std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> firstDiagVarRefs;
    for (unsigned int i = 1; i <= order; i += n + 1)
    {
        firstDiagVarRefs.emplace_back(squareToVarRefMap.at(i));
    }
    magicSquareConstrs.emplace_back(firstDiagVarRefs, exactMagicSumConstrEvaluator);
    
    std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> secondDiagVarRefs;
    for (unsigned int i = n; i < order; i += n - 1)
    {
        secondDiagVarRefs.emplace_back(squareToVarRefMap.at(i));
    }
    magicSquareConstrs.emplace_back(secondDiagVarRefs, exactMagicSumConstrEvaluator);
    
    std::vector<std::reference_wrapper<csp::Constraint<unsigned int>>> constraintsRefs{ magicSquareConstrs.begin(), magicSquareConstrs.end() };
    csp::ConstraintProblem<unsigned int> magicSquareProblem{ constraintsRefs, nameToVarRefMap };
    const csp::AssignmentHistory<unsigned int> magicSquareProbAssignmentHistory =
        csp::heuristicBacktrackingSolver<unsigned int>(magicSquareProb,
        csp::minimumRemainingValues_primarySelector<unsigned int>,
        csp::degreeHeuristic_secondarySelector<unsigned int>,
        csp::leastConstrainingValue<unsigned int>);
    magicSquareProb.writeNameToAssignment(std::cout);
    // to find all solutions see first example
    /*  RESULTS:
    1 : 8
    2 : 3
    3 : 4
    4 : 1
    5 : 5
    6 : 9
    7 : 6
    8 : 7
    9 : 2
    */
}
```
## Example #3: n-Queens
![](https://i.imgur.com/Ujq4LzZ.png)

Problem: place n queens on an n x n chessboard so that no two queens threaten each other.

Variables: columns of the board.  
Domain: the row each queen could be placed inside a column, i.e. each variable's domain is (1, ..., n).  
Constraints:  
1. The queens don't attack each other vertically.
2. The queens don't attack each other horizontally.
3. The queens don't attack each other diagonally.

Code implementation:
```cpp
#include <csp.h>

int main()
{
	std::vector<csp::Variable<unsigned int>> nQueensVars;
	std::vector<csp::Constraint<unsigned int>> nQueensConstrs;

	unsigned int n = 8;
	std::unordered_set<unsigned int> domain;
	domain.reserve(n);
	for (unsigned int i = 0; i < n; ++i)
	{
	    domain.emplace(i);
	}

	std::unordered_map<unsigned int, std::reference_wrapper<csp::Variable<unsigned int>>> columnToVarRefMap;
	std::unordered_map<std::string, std::reference_wrapper<csp::Variable<unsigned int>>> nameToVarRefMap;
	nQueensVars.reserve(n);
	for (unsigned int i = 0; i < n; ++i)
	{
	    nQueensVars.emplace_back(domain);
	    columnToVarRefMap.emplace(i, nQueensVars.back());
	    nameToVarRefMap.emplace(std::to_string(i), nQueensVars.back());
	}

	auto __uintCalcDifference = [](unsigned int first, unsigned int second) -> unsigned int
	{
            if (first < second)
                return second - first;
            else
                return first - second;
	};


	std::unordered_map<unsigned int, csp::ConstraintEvaluator<unsigned int>> notAttackingConstrainEvaluatorsMap;
	for (unsigned int columnDifference = 1; columnDifference < n; ++columnDifference)
	{
		notAttackingConstrainEvaluatorsMap.emplace(columnDifference,
		[&__uintCalcDifference, columnDifference](const std::vector<unsigned int>& assignedValues) -> bool
		{
			if (assignedValues.size() < 2)
			{
				return true;
			}
			unsigned int firstRow = assignedValues[0];
			unsigned int secondRow = assignedValues[1];
			return firstRow != secondRow && __uintCalcDifference(firstRow, secondRow) != columnDifference;
		});
	}

	nQueensConstrs.reserve(((n * n) + n) / 2);
	for (unsigned int firstCol = 0; firstCol < n; ++firstCol)
	{
	    for (unsigned int secondCol = 0; secondCol < n; ++secondCol)
	    {
		    if (firstCol < secondCol)
		    {
			    std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> varRefs
			    {
				   std::reference_wrapper<csp::Variable<unsigned int>>{ columnToVarRefMap.at(firstCol) },
				   std::reference_wrapper<csp::Variable<unsigned int>>{ columnToVarRefMap.at(secondCol) }
			    };
			    nQueensConstrs.emplace_back(varRefs,
			    notAttackingConstrainEvaluatorsMap[__uintCalcDifference(firstCol, secondCol)]);
		    }
	    }
    }

	std::vector<std::reference_wrapper<csp::Constraint<unsigned int>>> constraintsRefs{ nQueensConstrs.begin(), nQueensConstrs.end() };
	csp::ConstraintProblem<unsigned int> nQueensProblem{ constraintsRefs, nameToVarRefMap };

	const csp::AssignmentHistory<unsigned int> nQueensProbAssignmentHistory =
		csp::heuristicBacktrackingSolver<unsigned int>(nQueensProblem,
			csp::minimumRemainingValues_primarySelector<unsigned int>,
			csp::degreeHeuristic_secondarySelector<unsigned int>,
			csp::leastConstrainingValue<unsigned int>);
	nQueensProblem.writeNameToAssignment(std::cout);
    // to find all solutions see first example
    /*  RESULTS:
    0 : 3
    1 : 1
    2 : 6
    3 : 2
    4 : 5
    5 : 7
    6 : 4
    7 : 0
    */

}
```
Other examples that can be found under the 'examples' directory include:
1. Graph coloring.
2. Job scheduling.
3. Verbal arithmetic.
4. Magic Square.
5. Einstein's five houses riddle.
6. n-Queens.
7. Sudoku.

## Basic API (unsound and incomplete, made for explanatory purposes ONLY)
![](https://i.imgur.com/D5kheOt.jpg)
