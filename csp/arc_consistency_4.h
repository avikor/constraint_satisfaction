#pragma once

#include "pch.h"
#include "constraint_problem.h"

// Arc Consistency 4 (AC4) algorithm

namespace csp
{
	template <typename T>
	using VariableValueNeighborTriplet = std::tuple<Ref<csp::Variable<T>>, T, Ref<csp::Variable<T>>>;
}


namespace std
{
	template <typename T>
	struct std::hash<typename csp::VariableValuePair<T>>
	{
		size_t operator()(const csp::VariableValuePair<T>& variableValuePair) const
		{
			std::hash<T> valueHasher;
			std::hash<std::reference_wrapper<csp::Variable<T>>> varRefHasher;
			double hashValue = static_cast<double>(varRefHasher(get<0>(variableValuePair)));
			hashValue /= valueHasher(get<1>(variableValuePair));
			return static_cast<size_t>(hashValue);
		}
	};
}

namespace std
{
	template <typename T>
	struct std::hash<typename csp::VariableValueNeighborTriplet<T>>
	{
		size_t operator()(const csp::VariableValueNeighborTriplet<T>& variableValueNeighborTriplet) const
		{
			std::hash<T> valueHasher;
			std::hash<std::reference_wrapper<csp::Variable<T>>> varRefHasher;
			double hashValue = static_cast<double>(varRefHasher(get<0>(variableValueNeighborTriplet)));
			hashValue /= valueHasher(get<1>(variableValueNeighborTriplet));
			hashValue /= varRefHasher(get<2>(variableValueNeighborTriplet));
			return static_cast<size_t>(hashValue);
		}
	};
}

namespace csp
{
	template <typename T>
	static void __initialize_ac4(const std::vector<Ref<Constraint<T>>>& constraints,
		std::unordered_map<VariableValueNeighborTriplet<T>, int>& supportCounter,
		std::unordered_map<VariableValuePair<T>, std::unordered_set<VariableValuePair<T>>>& variableValuePairsSupportedBy,
		std::queue<VariableValuePair<T>>& unsupportedVariableValuePairs)
	{
		for (const Constraint<T>& constr : constraints)
		{
			const std::vector<Ref<Variable<T>>>& variables = constr.getVariables();
			if (variables.size() == 1)
			{
				continue;
			}
			Variable<T>* firstVar = &(variables[0].get());
			Variable<T>* secondVar = &(variables[1].get());

			for (int i = 0; i < 2; ++i)
			{
				bool firstVarWasAssigned = firstVar->isAssigned();
				const std::vector<T>& firstDomain = firstVar->getDomain();

				for (size_t i = 0; i < firstDomain.size(); ++i)
				{
					if (!firstVarWasAssigned)
					{
						firstVar->assignByIdx(i);
					}

					T firstValue = firstDomain[i];
					bool secondVarWasAssigned = secondVar->isAssigned();
					VariableValuePair<T> firstPair = std::make_pair(std::ref(*firstVar), firstValue);
					VariableValueNeighborTriplet<T> variableValueNeighborTriplet =
						std::make_tuple(std::ref(*firstVar), firstValue, std::ref(*secondVar));

					const std::vector<T>& secondDomain = secondVar->getDomain();
					for (size_t j = 0; j < secondDomain.size(); ++j)
					{
						if (!secondVarWasAssigned)
						{
							secondVar->assignByIdx(j);
						}
						if (constr.isConsistent())
						{
							supportCounter.try_emplace(variableValueNeighborTriplet, 0);
							++supportCounter[variableValueNeighborTriplet];
							VariableValuePair<T> secondPair = std::make_pair(std::ref(*secondVar), secondDomain[j]);
							variableValuePairsSupportedBy.try_emplace(secondPair, std::unordered_set<VariableValuePair<T>>{});
							variableValuePairsSupportedBy[secondPair].emplace(firstPair);
						}
						if (!secondVarWasAssigned)
						{
							secondVar->unassign();
						}
					}

					if (!firstVarWasAssigned)
					{
						firstVar->unassign();
					}
					if (!supportCounter[variableValueNeighborTriplet])
					{
						firstVar->removeFromDomainByIdx(i);
						unsupportedVariableValuePairs.emplace(firstPair);
					}
				}

				std::swap(firstVar, secondVar);
			}
		}
	}

	template <typename T>
	bool ac4(ConstraintProblem<T>& constraintProblem)
	{
		std::unordered_map<VariableValueNeighborTriplet<T>, int> supportCounter;
		std::unordered_map<VariableValuePair<T>, std::unordered_set<VariableValuePair<T>>> variableValuePairsSupportedBy;
		std::queue<VariableValuePair<T>> unsupportedVariableValuePairs;
		__initialize_ac4<T>(constraintProblem.getConstraints(), supportCounter, variableValuePairsSupportedBy, unsupportedVariableValuePairs);

		while (!unsupportedVariableValuePairs.empty())
		{
			const VariableValuePair<T>& secondVarValuePair = unsupportedVariableValuePairs.front();
			unsupportedVariableValuePairs.pop();
			for (const VariableValuePair<T>& firstVarValuePair : variableValuePairsSupportedBy[secondVarValuePair])
			{
				T firstValue = firstVarValuePair.second;
				Variable<T>& firstVar = firstVarValuePair.first;
				const std::vector<T>& firstDomain = firstVar.getDomain();
				const auto& firstDomainItEnd = firstDomain.cend();
				const auto& firstDomainItBegin = firstDomain.cbegin();
				auto serachRes = std::find(firstDomainItBegin, firstDomainItEnd, firstValue);
				if (serachRes != firstDomainItEnd)
				{
					VariableValueNeighborTriplet<T> variableValueNeighborTriplet =
						std::make_tuple(std::ref(firstVar), firstValue, std::ref(secondVarValuePair.first));
					--supportCounter[variableValueNeighborTriplet];
					if (!supportCounter[variableValueNeighborTriplet])
					{
						firstVar.removeFromDomainByIdx(std::distance(firstDomainItBegin, serachRes));
						unsupportedVariableValuePairs.emplace(firstVarValuePair);
					}
				}
			}
		}

		return constraintProblem.isPotentiallySolvable();
	}
}