#pragma once

#include "pch.h"

using CoordsToVarRefsUMap = std::unordered_map<std::pair<unsigned int, unsigned int>, std::reference_wrapper<csp::Variable<unsigned int>>>;


std::pair<csp::ConstraintProblem<unsigned int>, CoordsToVarRefsUMap> constructSudokuProblem(const char* filePath, 
	std::vector<csp::Variable<unsigned int>>& variables, std::vector<csp::Constraint<unsigned int>>& constraints);


std::string GetSudokuGridAsString(CoordsToVarRefsUMap& coordsToVarRefsUMap);