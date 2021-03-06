#pragma once

#include "pch.h"
#include "constraint_problem.h"


namespace csp
{
	template <typename T>
	static bool __backtrackingSolver(const ConstraintProblem<T>& constraintProblem, AssignmentHistory<T>& assignmentHistory,
		bool writeAssignmentHistory)
	{
		if (constraintProblem.isCompletelyAssigned())
		{
			if (constraintProblem.isConsistentlyAssigned())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		const std::vector<Ref<Variable<T>>> unassignedVars = constraintProblem.getUnassignedVariables();
		Variable<T>& selectedVar = unassignedVars.back();
		const std::vector<T>& selectedDomain = selectedVar.getDomain();
		for (size_t i = 0; i < selectedDomain.size(); ++i)
		{
			selectedVar.assignByIdx(i);
			if (writeAssignmentHistory)
			{
				assignmentHistory.emplace_back(selectedVar, std::optional<T>{ selectedDomain[i] });
			}

			if (__backtrackingSolver<T>(constraintProblem, assignmentHistory, writeAssignmentHistory))
			{
				return true;
			}

			selectedVar.unassign();
			if (writeAssignmentHistory)
			{
				assignmentHistory.emplace_back(selectedVar, std::optional<T>{});
			}
		}

		return false;
	}

	template <typename T>
	const AssignmentHistory<T> backtrackingSolver(const ConstraintProblem<T>& constraintProblem, bool writeAssignmentHistory = false)
	{
		AssignmentHistory<T> assignmentHistory;
		__backtrackingSolver<T>(constraintProblem, assignmentHistory, writeAssignmentHistory);
		return assignmentHistory;
	}

	template <typename T>
	void __backtrackingSolver_findAllSolutions(const ConstraintProblem<T>& constraintProblem, std::unordered_set<Assignment<T>>& solutions)
	{
		if (constraintProblem.isCompletelyAssigned())
		{
			if (constraintProblem.isConsistentlyAssigned())
			{
				solutions.emplace(constraintProblem.getCurrentAssignment());
			}
			else
			{
				return;
			}
		}

		const std::vector<Ref<Variable<T>>> unassignedVars = constraintProblem.getUnassignedVariables();
		if (unassignedVars.empty())
			return;

		Variable<T>& selectedVar = unassignedVars.back();
		const std::vector<T>& selectedDomain = selectedVar.getDomain();
		for (size_t i = 0; i < selectedDomain.size(); ++i)
		{
			selectedVar.assignByIdx(i);
			__backtrackingSolver_findAllSolutions<T>(constraintProblem, solutions);
			selectedVar.unassign();
		}
	}

	template <typename T>
	const std::unordered_set<Assignment<T>> backtrackingSolver_findAllSolutions(const ConstraintProblem<T>& constraintProblem)
	{
		std::unordered_set<Assignment<T>> solutions;
		__backtrackingSolver_findAllSolutions<T>(constraintProblem, solutions);
		return solutions;
	}
}