#pragma once

#include "pch.h"

csp::ConstraintProblem<unsigned int> constructSudokuProblem(unsigned int n, std::vector<csp::Variable<unsigned int>>& variables,
	std::vector<csp::Constraint<unsigned int>>& constraints);