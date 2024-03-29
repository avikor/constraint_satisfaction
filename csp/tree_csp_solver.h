#pragma once

#include "pch.h"
#include "constraint_problem.h"


namespace csp
{
	template <typename T>
	using DirectedGraph = std::unordered_map<Ref<Variable<T>>, std::unordered_set<Ref<Variable<T>>>>;

	template <typename T>
	static DirectedGraph<T> __init_directed_graph(ConstraintProblem<T>& constraintProblem, 
		const std::vector<Ref<Variable<T>>>& unassignedVariables)
	{
		DirectedGraph<T> directedGraph;
		for (Variable<T>& var : unassignedVariables)
		{
			for (Variable<T>& neighbor : constraintProblem.getUnassignedNeighbors(var))
			{
				directedGraph.try_emplace(neighbor, std::unordered_set<Ref<Variable<T>>>{});
				if (!(directedGraph[neighbor].count(var)))
				{
					directedGraph.try_emplace(var, std::unordered_set<Ref<Variable<T>>>{});
					directedGraph[var].emplace(neighbor);
				}
			}
		}
		
		return directedGraph;
	}

	template <typename T>
	std::unordered_map<Ref<Variable<T>>, size_t> __init_inDegree(DirectedGraph<T>& directedGraph, 
		const std::vector<Ref<Variable<T>>>& unassignedVariables)
	{
		std::unordered_map<Ref<Variable<T>>, size_t> inDegree;
		for (Variable<T>& var : unassignedVariables)
		{
			for (Variable<T>& neighbor : directedGraph[var])
			{
				inDegree.try_emplace(var, 0);
				++inDegree[neighbor];
			}
		}
		
		return inDegree;
	}

	template <typename T>
	std::unordered_set<Ref<Variable<T>>> init_zero_in_degree_vars(const std::unordered_map<Ref<Variable<T>>, size_t>& inDegree)
	{
		std::unordered_set<Ref<Variable<T>>> zeroInDegreeVars;
		zeroInDegreeVars.reserve(inDegree.size() >> 2);
		for (const std::pair<Ref<Variable<T>>, size_t>& varToInDegree : inDegree)
		{
			if (!varToInDegree.second)
			{
				zeroInDegreeVars.emplace(varToInDegree.first);
			}
		}

		return zeroInDegreeVars;
	}

	template <typename T>
	static const std::vector<Ref<Variable<T>>> __kahnTopologicalSort(ConstraintProblem<T>& constraintProblem)
	{
		const std::vector<Ref<Variable<T>>> unassignedVariables = constraintProblem.getUnassignedVariables();
		DirectedGraph<T> directedGraph = __init_directed_graph<T>(constraintProblem, unassignedVariables);
		std::unordered_map<Ref<Variable<T>>, size_t> inDegree = __init_inDegree<T>(directedGraph, unassignedVariables);
		std::unordered_set<Ref<Variable<T>>> zeroInDegreeVars = init_zero_in_degree_vars<T>(inDegree);

		std::vector<Ref<Variable<T>>> topologicalySortedUnassginedVars;
		topologicalySortedUnassginedVars.reserve(unassignedVariables.size());
		while (!zeroInDegreeVars.empty())
		{
			const auto zeroInDegreeVarsItToBegin = zeroInDegreeVars.cbegin();
			Variable<T>& var = *zeroInDegreeVarsItToBegin;
			zeroInDegreeVars.erase(zeroInDegreeVarsItToBegin);
			topologicalySortedUnassginedVars.emplace_back(var);
			for (Variable<T>& neighbor : directedGraph[var])
			{
				--inDegree[neighbor];
				if (!inDegree[neighbor])
				{
					zeroInDegreeVars.emplace(neighbor);
				}
			}
		}


		if (topologicalySortedUnassginedVars.size() != unassignedVariables.size())
		{
			return std::vector<Ref<Variable<T>>>{};
		}
		else
		{
			return topologicalySortedUnassginedVars;
		}
	}
}

namespace csp
{
	template <typename T>
	const AssignmentHistory<T> treeCspSolver(ConstraintProblem<T>& constraintProblem, bool writeAssignmentHistory = false)
	{
		AssignmentHistory<T> assignmentHistory;

		const std::vector<Ref<Variable<T>>> topologicalySortedUnassginedVars = __kahnTopologicalSort<T>(constraintProblem);
		if (topologicalySortedUnassginedVars.empty())
		{
			return assignmentHistory;
		}

		for (size_t i = topologicalySortedUnassginedVars.size() - 1; 0 < i; --i)
		{
			Variable<T>& currVariable = topologicalySortedUnassginedVars.at(i);
			const std::vector<T>& currDomain = currVariable.getDomain();
			for (auto it = currDomain.begin(); it != currDomain.end(); )
			{
				currVariable.assignByValue(*it);
				if (constraintProblem.getConsistentDomain(topologicalySortedUnassginedVars.at(i - 1)).empty())
				{
					currVariable.removeFromDomainByValue(*it);
				}
				else
				{
					++it;
				}
				currVariable.unassign();
			}

			if (currDomain.empty())
			{
				return assignmentHistory;
			}
		}

		for (Variable<T>& var : topologicalySortedUnassginedVars)
		{
			const std::vector<T> consistentDomain = constraintProblem.getConsistentDomain(var);
			if (consistentDomain.empty())
			{
				return assignmentHistory;
			}
			T value = consistentDomain.back();
			var.assignByValue(value);
			if (writeAssignmentHistory)
			{
				assignmentHistory.emplace_back(var, std::optional<T>{ value });
			}
		}

		return assignmentHistory;
	}
}