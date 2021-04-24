#include <csp.h>


// just some csp playground

int main()
{
    std::unordered_set<unsigned int> domain;
    domain.reserve(20);
    for (unsigned int i = 1; i < 21; ++i)
    {
        domain.insert(i);
    }

    csp::Variable<unsigned int> x{ domain };
    csp::Variable<unsigned int> y{ domain };
    csp::Variable<unsigned int> z{ domain };

    csp::Constraint<unsigned int> pythagoreanTripleConstraint{ { x, y, z },
        [](const std::vector<unsigned int>& assignedValues) -> bool
        {
            if (assignedValues.size() < 3)
            {
                return true;
            }
            unsigned int xVal = assignedValues.at(0);
            unsigned int yVal = assignedValues.at(1);
            unsigned int zVal = assignedValues.at(2);
            return xVal * xVal + yVal * yVal == zVal * zVal;
        } 
    };

    csp::Constraint<unsigned int> totalOrderConstraint{ { x, y, z },
        [](const std::vector<unsigned int>& assignedValues) -> bool
        {
            if (assignedValues.size() < 3)
            {
                return true;
            }
            return assignedValues.at(0) < assignedValues.at(1) && assignedValues.at(1) < assignedValues.at(2);
        }
    };

    csp::ConstraintProblem<unsigned int> pythagoreanTriplesProblem{ { pythagoreanTripleConstraint, totalOrderConstraint } };

    csp::backtrackingSolver<unsigned int>(pythagoreanTriplesProblem);
    std::cout << "x value: " << x.getValue() << '\n';
    std::cout << "y value: " << y.getValue() << '\n';
    std::cout << "z value: " << z.getValue() << '\n';
}