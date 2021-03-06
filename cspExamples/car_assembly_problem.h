#pragma once

#include "pch.h"

csp::ConstraintProblem<unsigned int> constructCarAssemblyProblem(std::vector<csp::Variable<unsigned int>>& variables,
	std::vector<csp::Constraint<unsigned int>>& constraints);