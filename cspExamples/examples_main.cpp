
#include "pch.h"
#include "australia_map_coloring_problem.h"
#include "n_queens_problem.h"
#include "magic_square_problem.h"
#include "car_assembly_problem.h"
#include "verbal_arithmetic_problem.h"
#include "einstein_five_house_riddle_problem.h"
#include "sudoku_problem.h"


int main()
{
	// uncomment to see solution
	/*std::vector<csp::Variable<std::string>> auMapColoringVars;
	std::vector<csp::Constraint<std::string>> auMapColoringConstrs;
	csp::ConstraintProblem<std::string> australiaMapColoringProb = constructAustraliaMapColoringProblem(auMapColoringVars, auMapColoringConstrs);
	std::cout << australiaMapColoringProb;
	std::cout << "\n----------------------------------------------------------------------------------------------------\n";
	const csp::AssignmentHistory<std::string> australiaMapColoringProbAssignmentHistory = 
		csp::heuristicBacktrackingSolver<std::string>(australiaMapColoringProb,
		csp::minimumRemainingValues_primarySelector<std::string>,
		csp::degreeHeuristic_secondarySelector<std::string>,
		csp::leastConstrainingValue<std::string>);
	australiaMapColoringProb.writeNameToAssignment(std::cout);*/

	// uncomment to see solution
	/*std::vector<csp::Variable<unsigned int>> nQueensVars;
	std::vector<csp::Constraint<unsigned int>> nQueensConstrs;
	csp::ConstraintProblem<unsigned int> nQueensProb = constructNQueensProblem(8, nQueensVars, nQueensConstrs);
	std::cout << nQueensProb;
	std::cout << "\n----------------------------------------------------------------------------------------------------\n";
	const csp::AssignmentHistory<unsigned int> nQueensProbAssignmentHistory = 
		csp::heuristicBacktrackingSolver<unsigned int>(nQueensProb,
		csp::minimumRemainingValues_primarySelector<unsigned int>,
		csp::degreeHeuristic_secondarySelector<unsigned int>,
		csp::leastConstrainingValue<unsigned int>);
	nQueensProb.writeNameToAssignment(std::cout);*/

	// uncomment to see solution
	/*std::vector<csp::Variable<unsigned int>> magicSquareVars;
	std::vector<csp::Constraint<unsigned int>> magicSquareConstrs;
	csp::ConstraintProblem<unsigned int> magicSquareProb = constructMagicSquareProblem(3, magicSquareVars, magicSquareConstrs);
	std::cout << magicSquareProb;
	std::cout << "\n----------------------------------------------------------------------------------------------------\n";
	const csp::AssignmentHistory<unsigned int> magicSquareProbAssignmentHistory =
		csp::heuristicBacktrackingSolver<unsigned int>(magicSquareProb,
		csp::minimumRemainingValues_primarySelector<unsigned int>,
		csp::degreeHeuristic_secondarySelector<unsigned int>,
		csp::leastConstrainingValue<unsigned int>);
	magicSquareProb.writeNameToAssignment(std::cout);*/

	// uncomment to see solution
	/*std::vector<csp::Variable<unsigned int>> carAssemblyVars;
	std::vector<csp::Constraint<unsigned int>> carAssemblyConstrs;
	csp::ConstraintProblem<unsigned int> carAssemblyProb = constructCarAssemblyProblem(carAssemblyVars, carAssemblyConstrs);
	std::cout << carAssemblyProb;
	std::cout << "\n----------------------------------------------------------------------------------------------------\n";
	const csp::AssignmentHistory<unsigned int> carAssemblyProbAssignmentHistory =
		csp::heuristicBacktrackingSolver<unsigned int>(carAssemblyProb,
		csp::minimumRemainingValues_primarySelector<unsigned int>,
		csp::degreeHeuristic_secondarySelector<unsigned int>,
		csp::leastConstrainingValue<unsigned int>);
	carAssemblyProb.writeNameToAssignment(std::cout);*/

	// uncomment to see solution
	/*std::vector<csp::Variable<unsigned int>> verbalArithmeticVars;
	std::vector<csp::Constraint<unsigned int>> verbalArithmeticConstraints;
	csp::ConstraintProblem<unsigned int> verbalArithmeticProb = constructVerbalArithmeticProblem(verbalArithmeticVars, verbalArithmeticConstraints);
	std::cout << verbalArithmeticProb;
	std::cout << "\n----------------------------------------------------------------------------------------------------\n";
	const csp::AssignmentHistory<unsigned int> verbalArithmeticProbAssignmentHistory =
		csp::heuristicBacktrackingSolver<unsigned int>(verbalArithmeticProb,
			csp::minimumRemainingValues_primarySelector<unsigned int>,
			csp::degreeHeuristic_secondarySelector<unsigned int>,
			csp::leastConstrainingValue<unsigned int>);
	verbalArithmeticProb.writeNameToAssignment(std::cout);*/

	// uncomment to see solution
	/*std::vector<csp::Variable<std::string>> einsteinRiddleVars;
	std::vector<csp::Constraint<std::string>> einsteinRiddleConstraints;
	VarRefsMaps varsRefMaps;
	csp::ConstraintProblem<std::string> einsteinRiddleProb = constructEinsteinRiddleProblem(einsteinRiddleVars, 
		einsteinRiddleConstraints, varsRefMaps);
	std::cout << einsteinRiddleProb;
	std::cout << "\n----------------------------------------------------------------------------------------------------\n";
	const csp::AssignmentHistory<std::string> einsteinRiddleProbAssignmentHistory =
		csp::heuristicBacktrackingSolver<std::string>(einsteinRiddleProb,
			csp::minimumRemainingValues_primarySelector<std::string>,
			csp::degreeHeuristic_secondarySelector<std::string>,
			csp::leastConstrainingValue<std::string>);
	for (unsigned int i = 1; i < 6; ++i)
	{
		std::cout << i << " : " << varsRefMaps.colorsVarsMap.at(i).get().getValue() << ' ';
		std::cout << varsRefMaps.nationalitiesVarsMap.at(i).get().getValue() << ' ';
		std::cout << varsRefMaps.drinksVarsMap.at(i).get().getValue() << ' ';
		std::cout << varsRefMaps.smokesVarsMap.at(i).get().getValue() << ' ';
		std::cout << varsRefMaps.petsVarsMap.at(i).get().getValue() << '\n';
	}*/


	std::vector<csp::Variable<unsigned int>> variables;
	std::vector<csp::Constraint<unsigned int>> constraints;
	std::pair<csp::ConstraintProblem<unsigned int>, CoordsToVarRefsUMap> res = constructSudokuProblem("sudoku_puzzles/9x9_easy.txt", variables, constraints);
	csp::ConstraintProblem<unsigned int>& sudokuProb = res.first;
	const csp::AssignmentHistory<unsigned int> sudokuProbAssignmentHistory =
		csp::heuristicBacktrackingSolver<unsigned int>(sudokuProb,
			csp::minimumRemainingValues_primarySelector<unsigned int>,
			csp::degreeHeuristic_secondarySelector<unsigned int>,
			csp::leastConstrainingValue<unsigned int>);

	std::string strGrid = GetSudokuGridAsString(res.second);
	std::cout << strGrid;

	/* 
	example for sudoku_puzzles/25x25_easy.txt
	11  3  2 17 23 | 13 12 25  1  4 | 21  7  9 15 16 | 24 22  8 18 20 | 14  6 10 19  5
	10  5 16  7 24 | 14  2  8 23 18 | 22  3 17 13 12 | 15 21  6  4 19 |  9 20  1 11 25
	 1 22  9 12 19 |  3 15  6 11 16 |  5 10  2 20 14 | 25 17 23  7 13 | 18  8 21  4 24
	25 20  8 13 18 |  7 21 19 10  9 | 23 24 11  4  6 | 16  5  1 12 14 |  2 15 22  3 17
	14  4  6 21 15 |  5 24 22 17 20 | 25  1 18  8 19 |  2 11  9  3 10 | 13  7 16 23 12
	-  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -
	12  9 25 11  8 |  2 20 16 24  3 | 10  4 23  6 13 | 17  7 21  5 18 | 22 19 14  1 15
	 4 14 18 24 17 | 15 10  1 21 23 | 16 12 22 11  3 | 13  6  2 19  9 | 25  5  8 20  7
	16 19 22  5  7 |  9  8 11 18  6 | 20 14 15 24 21 |  4 10 25  1 12 | 17 23 13  2  3
	15 21 23 10  1 | 12  5 13  4 17 | 18 19 25  7  2 | 14 20  3 22  8 |  6 16 24  9 11
	20  2  3  6 13 | 25 14  7 22 19 |  1  8  5 17  9 | 23 16 15 11 24 | 21 10 18 12  4
	-  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -
	19 11 24  3  9 | 10  4 18  8  2 | 17 22 14 12  1 | 20 13  7 23  5 | 16 21 25 15  6
	21 18  4 22 14 | 16 17  5 12  7 | 11  6 20  9 15 |  8  3 10 24 25 | 23  2 19 13  1
	17  1 13 15  6 | 20  9 23 25 22 |  3 16  4  5  8 | 19 12 11 21  2 | 24 18  7 10 14
	 8  7 10 16  2 | 21  6 24  3  1 | 13 23 19 25 18 |  9  4 14 15 17 |  5 11 12 22 20
	 5 25 20 23 12 | 11 19 15 13 14 | 24 21 10  2  7 |  1 18 16  6 22 |  3 17  4  8  9
	-  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -
	 3 24  7  9 16 |  4  1 12 19 21 | 14  2  6 10  5 | 11  8 18 20 23 | 15 22 17 25 13
	22 15 21 14  4 | 18  3 17 16 11 | 19 13  7 23 20 | 10 25 12  9  1 |  8 24  5  6  2
	13  6  5 20 25 | 22 23  9 15 10 |  8 18  3 21 17 |  7  2 24 14  4 | 12  1 11 16 19
	 2 23 12 19 10 | 24  7 20  5  8 |  9 25  1 16 11 | 22 15 13 17  6 |  4 14  3 18 21
	18 17  1  8 11 |  6 25  2 14 13 | 12 15 24 22  4 | 21 19  5 16  3 | 20  9 23  7 10
	-  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -  + -  -  -  -  -
	23 13 11  1  5 |  8 18 10  6 25 |  4 20 21 14 22 |  3  9 17  2  7 | 19 12 15 24 16
	 6 16 14 25 21 | 19 11  4  7 24 |  2 17  8 18 10 | 12 23 20 13 15 |  1  3  9  5 22
	 7  8 17  2 20 |  1 16  3  9 12 | 15  5 13 19 24 | 18 14 22 25 11 | 10  4  6 21 23
	 9 12 19 18  3 | 23 22 14  2 15 |  6 11 16  1 25 |  5 24  4 10 21 |  7 13 20 17  8
	24 10 15  4 22 | 17 13 21 20  5 |  7  9 12  3 23 |  6  1 19  8 16 | 11 25  2 14 18
	*/
}