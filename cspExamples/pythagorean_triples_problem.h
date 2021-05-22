#pragma once

#include "pch.h"

csp::ConstraintProblem<int> constructPythagoreanTriplesProblem(int n, std::vector<csp::Variable<int>>& variables,
	std::vector<csp::Constraint<int>>& constraints);