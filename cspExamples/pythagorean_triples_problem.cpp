#include "pch.h"
#include "pythagorean_triples_problem.h"

csp::ConstraintProblem<int> constructPythagoreanTriplesProblem(int n, std::vector<csp::Variable<int>>& variables,
	std::vector<csp::Constraint<int>>& constraints)
{
    std::unordered_set<int> domain;
    domain.reserve(n);
    for (int i = 1; i < n + 1; ++i)
        domain.insert(i);

    const std::unordered_set<std::string> names{ "x", "y", "z" };
    std::unordered_map<std::string, std::reference_wrapper<csp::Variable<int>>> nameToVarRefMap =
        csp::Variable<int>::constructFromNamesToEqualDomainPutInVec(names, domain, variables);

    std::vector<std::reference_wrapper<csp::Variable<int>>> variablesRefs;
    for (const auto& [name, varRef] : nameToVarRefMap)
        variablesRefs.push_back(varRef);

    csp::Constraint<int> pythagoreanTripleConstraint{ variablesRefs,
        [](const std::vector<int>& assignedValues) -> bool
        {
            if (assignedValues.size() < 3)
            {
                return true;
            }
            int xVal = assignedValues.at(0);
            int yVal = assignedValues.at(1);
            int zVal = assignedValues.at(2);
            return xVal * xVal + yVal * yVal == zVal * zVal;
        }
    };

    csp::Constraint<int> totalOrderConstraint{ variablesRefs,
        [](const std::vector< int>& assignedValues) -> bool
        {
            if (assignedValues.size() < 3)
            {
                return true;
            }
            return assignedValues.at(0) < assignedValues.at(1) && assignedValues.at(1) < assignedValues.at(2);
        }
    };

    constraints.push_back(pythagoreanTripleConstraint);
    constraints.push_back(totalOrderConstraint);

    std::vector<std::reference_wrapper<csp::Constraint<int>>> constraintsRefs{ constraints.begin(), constraints.end() };
    csp::ConstraintProblem<int> pythagoreanTriplesProblem{ constraintsRefs, nameToVarRefMap };

    return pythagoreanTriplesProblem;
}