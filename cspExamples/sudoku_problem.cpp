#include "pch.h"
#include "sudoku_problem.h"



namespace std
{
	template<>
	struct std::hash<std::pair<unsigned int, unsigned int>>
	{
		size_t operator()(const std::pair<unsigned int, unsigned int>& unsignedIntsPair) const
		{
			std::hash<unsigned int> unsignedIntHasher;
			return static_cast<size_t>(unsignedIntHasher(unsignedIntsPair.first) / unsignedIntHasher(unsignedIntsPair.second));
		}
	};
}

std::vector<std::vector<unsigned int>> static __parse_sudoku_board(const char* filePath)
{
	// CSPDO: write in each text file the dimension/length of the sudoku grid/board, that way you can reserve space in advance

	std::ifstream input{ filePath };
	if (!input.is_open())
	{
		throw std::runtime_error("Could not open file.");
	}

	std::vector<std::vector<unsigned int>> grid;

	for (std::string line; std::getline(input, line); )
	{
		grid.emplace_back(std::vector<unsigned int>{ });
		std::vector<unsigned int>& currLine = grid.back();

		std::istringstream tokenStream(line);
		for (std::string integerAsStr; std::getline(tokenStream, integerAsStr, ' '); )
		{
			currLine.push_back(std::stoi(integerAsStr));
		}
	}

	input.close();
	return grid;
}

static std::unordered_set<unsigned int> __init_domain(unsigned int gridLen)
{
	std::unordered_set<unsigned int> domain;
	domain.reserve(gridLen);
	for (unsigned int k = 1; k <= gridLen; ++k)
	{
		domain.emplace(k);
	}
	return domain;
}


static CoordsToVarRefsUMap __init_variables(std::vector<csp::Variable<unsigned int>>& variables, 
	const std::vector<std::vector<unsigned int>>& grid)
{
	unsigned int gridLen = grid.size();
	variables.reserve(gridLen * gridLen);
	CoordsToVarRefsUMap coordsToVarRefsUMap;
	coordsToVarRefsUMap.reserve(gridLen * gridLen);

	std::unordered_set<unsigned int> domain = __init_domain(gridLen);

	for (unsigned int i = 0; i < gridLen; ++i)
	{
		for (unsigned int j = 0; j < gridLen; ++j)
		{
			if (grid[i][j])
			{
				variables.emplace_back(std::unordered_set<unsigned int>{ grid[i][j] });
			}
			else
			{
				variables.emplace_back(domain);
			}

			std::pair<unsigned int, unsigned int> coords{ i, j };
			coordsToVarRefsUMap.emplace(coords, variables.back());
		}
	}
	return coordsToVarRefsUMap;
}

static std::vector<std::vector<std::pair<unsigned int, unsigned int>>> __get_row_indices(unsigned int gridLen)
{
	std::vector<std::vector<std::pair<unsigned int, unsigned int>>> rowIndices;
	rowIndices.reserve(gridLen);
	for (unsigned int i = 0; i < gridLen; ++i)
	{
		std::vector<std::pair<unsigned int, unsigned int>> rowIdxs;
		rowIdxs.reserve(gridLen);
		for (unsigned int j = 0; j < gridLen; ++j)
		{
			rowIdxs.emplace_back(i, j);
		}
		rowIndices.emplace_back(rowIdxs);
	}
	return rowIndices;
}

static std::vector<std::vector<std::pair<unsigned int, unsigned int>>> __get_column_indices(unsigned int gridLen)
{
	std::vector<std::vector<std::pair<unsigned int, unsigned int>>> colIndices;
	colIndices.reserve(gridLen);
	for (unsigned int i = 0; i < gridLen; ++i)
	{
		std::vector<std::pair<unsigned int, unsigned int>> colIdxs;
		colIdxs.reserve(gridLen);
		for (unsigned int j = 0; j < gridLen; ++j)
		{
			colIdxs.emplace_back(j, i);
		}
		colIndices.emplace_back(colIdxs);
	}
	return colIndices;
}

static std::vector<std::vector<std::pair<unsigned int, unsigned int>>> __get_block_indices(unsigned int gridLen)
{
	unsigned int blockLen = static_cast<unsigned int>(std::sqrt(gridLen));
	std::vector<std::vector<std::pair<unsigned int, unsigned int>>> blockIndices;
	blockIndices.reserve(gridLen);

	std::vector<std::pair<unsigned int, unsigned int>> blocksStartingIndices;
	blocksStartingIndices.reserve(gridLen);
	for (unsigned int i = 0; i < gridLen; i += blockLen)
	{
		for (unsigned int j = 0; j < gridLen; j += blockLen)
		{
			blocksStartingIndices.emplace_back(i, j);
		}
	}

	for (std::pair<unsigned int, unsigned int>& blockStartIndices : blocksStartingIndices)
	{
		std::vector<std::pair<unsigned int, unsigned int>> blockIdxs;
		blockIdxs.reserve(gridLen);
		for (unsigned int i = blockStartIndices.first; i < blockStartIndices.first + blockLen; ++i)
		{
			for (unsigned int j = blockStartIndices.second; j < blockStartIndices.second + blockLen; ++j)
			{
				blockIdxs.emplace_back(i, j);
			}
		}
		blockIndices.emplace_back(blockIdxs);
	}
	return blockIndices;
}

static void __init_constraints(std::vector<csp::Variable<unsigned int>>& variables,
	std::vector<csp::Constraint<unsigned int>>& constraints, CoordsToVarRefsUMap& coordsToVarRefsUMap)
{
	unsigned int gridLen = static_cast<unsigned int>(std::sqrt(variables.size()));
	std::vector<std::vector<std::pair<unsigned int, unsigned int>>> rowIndices = __get_row_indices(gridLen);
	std::vector<std::vector<std::pair<unsigned int, unsigned int>>> colIndices = __get_column_indices(gridLen);
	std::vector<std::vector<std::pair<unsigned int, unsigned int>>> blockIndices = __get_block_indices(gridLen);

	for (std::vector<std::pair<unsigned int, unsigned int>>& row : rowIndices)
	{
		std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> rowVars;
		rowVars.reserve(gridLen);
		for (std::pair<unsigned int, unsigned int>& rowIdx : row)
		{
			rowVars.emplace_back(coordsToVarRefsUMap.at(rowIdx));
		}
		constraints.emplace_back(rowVars, csp::allDiff<unsigned int>);
	}


	for (std::vector<std::pair<unsigned int, unsigned int>>& col : colIndices)
	{
		std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> colVars;
		colVars.reserve(gridLen);
		for (std::pair<unsigned int, unsigned int>& colIdx : col)
		{
			colVars.emplace_back(coordsToVarRefsUMap.at(colIdx));
		}
		constraints.emplace_back(colVars, csp::allDiff<unsigned int>);
	}

	for (std::vector<std::pair<unsigned int, unsigned int>>& block : blockIndices)
	{
		std::vector<std::reference_wrapper<csp::Variable<unsigned int>>> blockVars;
		blockVars.reserve(gridLen);
		for (std::pair<unsigned int, unsigned int>& blockIdx : block)
		{
			blockVars.emplace_back(coordsToVarRefsUMap.at(blockIdx));
		}
		constraints.emplace_back(blockVars, csp::allDiff<unsigned int>);
	}
}

std::pair<csp::ConstraintProblem<unsigned int>, CoordsToVarRefsUMap> constructSudokuProblem(const char* filePath, std::vector<csp::Variable<unsigned int>>& variables,
	std::vector<csp::Constraint<unsigned int>>& constraints)
{
	std::vector<std::vector<unsigned int>> grid = __parse_sudoku_board(filePath);
	CoordsToVarRefsUMap coordsToVarRefsUMap = __init_variables(variables, grid);
	__init_constraints(variables, constraints, coordsToVarRefsUMap);

	std::vector<std::reference_wrapper<csp::Constraint<unsigned int>>> constraintsRefs{ constraints.begin(), constraints.end() };
	csp::ConstraintProblem<unsigned int> sudokuProblem{ constraintsRefs };
	return { sudokuProblem, coordsToVarRefsUMap };
}

std::string GetSudokuGridAsString(CoordsToVarRefsUMap& coordsToVarRefsUMap)
{
	unsigned int gridLen = static_cast<unsigned int>(std::sqrt(coordsToVarRefsUMap.size()));
	std::vector<std::vector<unsigned int>> grid;
	grid.reserve(gridLen);
	for (unsigned int i = 0; i < gridLen; ++i)
	{
		grid.push_back(std::vector<unsigned int>(gridLen, 0));
	}
	
	for (auto& coordsToVar : coordsToVarRefsUMap)
	{
		const std::pair<unsigned int, unsigned int>& coords = coordsToVar.first;
		csp::Variable<unsigned int>& var = coordsToVar.second;
		if (var.isAssigned())
		{
			grid[coords.first][coords.second] = var.getValue();
		}
		else
		{
			grid[coords.first][coords.second] = 0;
		}
	}

	unsigned int blockLen = static_cast<unsigned int>(std::sqrt(gridLen));
	std::unordered_set<unsigned int> midIndices;
	midIndices.reserve(gridLen);
	for (unsigned int i = blockLen; i < gridLen; i += blockLen)
	{
		midIndices.insert(i);
	}

	std::vector<std::string> gridStrings;
	gridStrings.reserve(gridLen);

	for (unsigned int i = 0; i < gridLen; ++i)
	{
		if (midIndices.count(i))
		{
			std::string str;
			for (unsigned int j = 0; j < blockLen; ++j)
			{
				str += "-  ";
			}
			for (unsigned int k = 0; k < blockLen; ++k)
			{
				gridStrings.push_back(str);
				if (k != blockLen - 1)
				{
					gridStrings.push_back("+ ");
				}
			}
			gridStrings.push_back("\n");
		}

		for (unsigned int j = 0; j < gridLen; ++j)
		{
			if (midIndices.count(j))
			{
				gridStrings.push_back("| ");
			}
			if (grid[i][j] < 10)
			{
				gridStrings.push_back(" " + std::to_string(grid[i][j]) + " ");
			}
			else
			{
				gridStrings.push_back(std::to_string(grid[i][j]) + " ");
			}
		}

		gridStrings.push_back("\n");
	}

	std::string strGrid = std::accumulate(gridStrings.begin(), gridStrings.end(), std::string(""));
	return strGrid;
}


// CSPDO:
// implement SudokuStartStateGenerator
// implement sudoku_score_calculator
// implement SudokuSuccessorGenerator
// implement GeneticSudokuConstraintProblem