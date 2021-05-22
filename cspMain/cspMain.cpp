#include <csp.h>


// just some csp playground

int main()
{
    constexpr size_t n = 20;
    std::unordered_set<int> domain;
    domain.reserve(n);
    for (int i = 1; i < n + 1; ++i)
        domain.insert(i);

    std::vector<csp::Variable<int>> variables;
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

    std::vector<std::reference_wrapper<csp::Constraint<int>>> constRefs{ pythagoreanTripleConstraint, totalOrderConstraint };



    csp::ConstraintProblem<int> pythagoreanTriplesProblem{constRefs, nameToVarRefMap };

    /*csp::backtrackingSolver<unsigned int>(pythagoreanTriplesProblem);
    std::cout << "x value: " << x.getValue() << '\n';
    std::cout << "y value: " << y.getValue() << '\n';
    std::cout << "z value: " << z.getValue() << '\n';*/

    /*const std::unordered_set<csp::Assignment<int>> backSols = csp::backtrackingSolver_findAllSolutions<int>(pythagoreanTriplesProblem);
    for (const csp::Assignment<int>& assignment : backSols)
    {
        pythagoreanTriplesProblem.unassignAllVariables();
        pythagoreanTriplesProblem.assignFromAssignment(assignment);
        pythagoreanTriplesProblem.writeNameToAssignment(std::cout);
        std::cout << '\n';
    }*/

    const std::unordered_set<csp::Assignment<int>> sols = csp::heuristicBacktrackingSolver_findAllSolutions<int>(
        pythagoreanTriplesProblem,
        csp::minimumRemainingValues_primarySelector<int>,
        csp::degreeHeuristic_secondarySelector<int>,
        csp::leastConstrainingValue<int>);

    std::cout << "number of solutions: " << sols.size() << '\n';

    /*for (const csp::Assignment<int>& assignment : sols)
    {
        pythagoreanTriplesProblem.unassignAllVariables();
        pythagoreanTriplesProblem.assignFromAssignment(assignment);
        std::string strJson = pythagoreanTriplesProblem.writeNameToAssignmentJSON();
        std::cout << strJson;
        std::cout << '\n';
    }*/
}