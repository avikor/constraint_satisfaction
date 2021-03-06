#include "pch.h"
#include "CppUnitTest.h"
#include <csp.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace cspTests
{
	TEST_CLASS(ConstraintProblemTests)
	{
	public:

		const std::unordered_set<std::string> domain{ "Red", "Green", "Blue" };
		const std::unordered_set<std::string> names{ "nt", "q", "nsw", "v", "t", "sa", "wa" };
		std::unordered_map<std::string, csp::Variable<std::string>> NameToVarUMap = csp::Variable<std::string>::constructFromNamesToEqualDomain(names, domain);

		csp::Constraint<std::string> constr1{ { NameToVarUMap.at("sa"), NameToVarUMap.at("wa") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr2{ { NameToVarUMap.at("sa"), NameToVarUMap.at("nt") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr3{ { NameToVarUMap.at("sa"), NameToVarUMap.at("q") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr4{ { NameToVarUMap.at("sa"), NameToVarUMap.at("nsw") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr5{ { NameToVarUMap.at("sa"), NameToVarUMap.at("v") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr6{ { NameToVarUMap.at("wa"), NameToVarUMap.at("nt") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr7{ { NameToVarUMap.at("nt"), NameToVarUMap.at("q") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr8{ { NameToVarUMap.at("q"), NameToVarUMap.at("nsw") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr9{ { NameToVarUMap.at("nsw"), NameToVarUMap.at("v") }, csp::allDiff<std::string> };
		csp::Constraint<std::string> constr10{ { NameToVarUMap.at("t")}, csp::allDiff<std::string> };

		csp::ConstraintProblem<std::string> graphColoringProb{ {constr1, constr2, constr3, constr4, constr5, constr6, constr7, constr8,
			constr9, constr10} };

		std::vector<std::reference_wrapper<csp::Constraint<std::string>>> constraints{ constr1, constr2, constr3, constr4, constr5, constr6,
		constr7, constr8, constr9, constr10 };

		TEST_METHOD_INITIALIZE(ConstraintProblemSetUp)
		{
			graphColoringProb.unassignAllVariables();
		}

		TEST_METHOD(TestCopyCtor)
		{
			std::vector<csp::ConstraintProblem<std::string>> vec;
			vec.push_back(graphColoringProb);
			Assert::IsTrue(vec.size() == 1);
			Assert::IsTrue(vec.back() != graphColoringProb);
		}

		TEST_METHOD(TestCopyAssignmentOperator)
		{
			csp::ConstraintProblem<std::string> constrProb17{ {constr1, constr2, constr3} };
			constrProb17 = graphColoringProb;
			Assert::IsTrue(constrProb17.getConstraints().size() == 10);
		}

		TEST_METHOD(TestMoveCtor)
		{
			csp::ConstraintProblem<std::string> constrProb7{ {constr1, constr2, constr3} };
			csp::ConstraintProblem<std::string> constrProb8{ std::move(constrProb7) };
			Assert::IsTrue(constrProb7.getVariables().empty());
			Assert::IsTrue(constrProb7.getConstraints().empty());
			Assert::IsTrue(constrProb8.getVariables().size() == 4);
			Assert::IsTrue(constrProb8.getConstraints().size() == 3);
		}

		TEST_METHOD(TestMoveCtor2)
		{
			std::vector<csp::Variable<std::string>> copiedVars;
			std::vector<csp::Constraint<std::string>> copiedConstraints;
			csp::ConstraintProblem<std::string> copiedGraphColoringProb = graphColoringProb.deepCopy(copiedVars, copiedConstraints);

			csp::ConstraintProblem<std::string> movedProb{ std::move(copiedGraphColoringProb) };
			Assert::IsTrue(copiedGraphColoringProb.getVariables().size() == 0);
			Assert::IsTrue(copiedGraphColoringProb.getConstraints().size() == 0);
			Assert::IsTrue(movedProb.getVariables().size() == 7);
			Assert::IsTrue(movedProb.getConstraints().size() == 10);
		}

		TEST_METHOD(TestMoveAssignmentOperator)
		{
			csp::ConstraintProblem<std::string> constrProb13{ {constr1, constr2, constr3} };
			csp::ConstraintProblem<std::string> constrProb14{ {constr4, constr5, constr6, constr7, constr8, constr9} };
			constrProb13 = std::move(constrProb14);
			Assert::IsTrue(constrProb13.getConstraints().size() == 6);
			Assert::IsTrue(constrProb14.getConstraints().size() == 3);
		}

		TEST_METHOD(TestMoveAssignmentOperator2)
		{
			std::vector<csp::Variable<std::string>> copiedVars;
			std::vector<csp::Constraint<std::string>> copiedConstraints;
			csp::ConstraintProblem<std::string> copiedGraphColoringProb = graphColoringProb.deepCopy(copiedVars, copiedConstraints);

			csp::ConstraintProblem<std::string> movedProb = std::move(copiedGraphColoringProb);
			Assert::IsTrue(copiedGraphColoringProb.getVariables().size() == 0);
			Assert::IsTrue(copiedGraphColoringProb.getConstraints().size() == 0);
			Assert::IsTrue(movedProb.getVariables().size() == 7);
			Assert::IsTrue(movedProb.getConstraints().size() == 10);
		}

		TEST_METHOD(TestDeepCopy)
		{
			graphColoringProb.assignVarsWithRandomValues();
			std::vector<csp::Variable<std::string>> copiedVars;
			std::vector<csp::Constraint<std::string>> copiedConstraints;
			csp::ConstraintProblem<std::string> copiedGraphColoringProb = graphColoringProb.deepCopy(copiedVars, copiedConstraints);
			Assert::IsTrue(copiedGraphColoringProb.getVariables().size() == 7);
			Assert::IsTrue(copiedGraphColoringProb.getConstraints().size() == 10);
			int twoConstrVarsSizeCounter = 0;
			int oneConstrVarsSizeCounter = 0;
			for (const csp::Constraint<std::string>& constr : copiedConstraints)
			{
				if (constr.getVariables().size() == 2)
				{
					++twoConstrVarsSizeCounter;
				}
				else if (constr.getVariables().size() == 1)
				{
					++oneConstrVarsSizeCounter;
				}
			}
			Assert::IsTrue(twoConstrVarsSizeCounter == 9);
			Assert::IsTrue(oneConstrVarsSizeCounter == 1);

			for (const csp::Variable<std::string>& var : copiedVars)
			{
				Assert::IsTrue(var.getDomain().size() == 3);
			}

			for (const csp::Variable<std::string>& varCpy : copiedVars)
			{
				for (const csp::Variable<std::string>& varOrg : graphColoringProb.getVariables())
				{
					Assert::IsTrue(varCpy != varOrg);
				}
			}

			for (const csp::Constraint<std::string>& constrCpy : copiedConstraints)
			{
				for (const csp::Constraint<std::string>& constrOrg : graphColoringProb.getConstraints())
				{
					Assert::IsTrue(constrCpy != constrOrg);
				}
			}

			Assert::IsTrue(copiedGraphColoringProb != graphColoringProb);
		}

		TEST_METHOD(TestIsCompletelyUnassigned)
		{
			Assert::IsTrue(graphColoringProb.isCompletelyUnassigned());
			NameToVarUMap.at("sa").assignByValue("Green");
			Assert::IsFalse(graphColoringProb.isCompletelyUnassigned());
			NameToVarUMap.at("sa").unassign();
		}

		TEST_METHOD(TestIsConsistentlyAssigned)
		{
			Assert::IsTrue(graphColoringProb.isConsistentlyAssigned());
			NameToVarUMap.at("t").assignByValue("Red");
			Assert::IsTrue(graphColoringProb.isConsistentlyAssigned());
			NameToVarUMap.at("t").unassign();
		}

		TEST_METHOD(TestIsCompletelyAssigned)
		{
			Assert::IsFalse(graphColoringProb.isCompletelyAssigned());
			for (std::pair<const std::string, csp::Variable<std::string>>& nameToVar : NameToVarUMap)
			{
				nameToVar.second.assignByValue("Green");
			}
			Assert::IsTrue(graphColoringProb.isCompletelyAssigned());
		}

		TEST_METHOD(TestIsCompletelyConsistentlyAssigned)
		{
			Assert::IsFalse(graphColoringProb.isCompletelyConsistentlyAssigned());
			for (std::pair<const std::string, csp::Variable<std::string>>& nameToVar : NameToVarUMap)
			{
				nameToVar.second.assignByValue("Green");
			}
			Assert::IsFalse(graphColoringProb.isCompletelyConsistentlyAssigned());
			graphColoringProb.unassignAllVariables();
			NameToVarUMap.at("wa").assignByValue("Red");
			NameToVarUMap.at("nt").assignByValue("Green");
			NameToVarUMap.at("sa").assignByValue("Blue");
			NameToVarUMap.at("q").assignByValue("Red");
			NameToVarUMap.at("nsw").assignByValue("Green");
			NameToVarUMap.at("v").assignByValue("Red");
			NameToVarUMap.at("t").assignByValue("Blue");
			Assert::IsTrue(graphColoringProb.isCompletelyConsistentlyAssigned());
		}

		TEST_METHOD(TestGetVariables)
		{
			const std::vector<std::reference_wrapper<csp::Variable<std::string>>> graphColoringVars = graphColoringProb.getVariables();
			for (std::pair<const std::string, csp::Variable<std::string>>& nameToVar : NameToVarUMap)
			{
				Assert::IsTrue(find(graphColoringVars.cbegin(), graphColoringVars.cend(),
					nameToVar.second) != graphColoringVars.cend());
			}
		}

		TEST_METHOD(TestGetConstraints)
		{
			Assert::IsTrue(constraints == graphColoringProb.getConstraints());
		}

		TEST_METHOD(TestGetAssignedVariables)
		{
			NameToVarUMap.at("q").assignByValue("Red");
			NameToVarUMap.at("t").assignByValue("Blue");
			NameToVarUMap.at("v").assignByValue("Red");
			std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> assignedVars{ NameToVarUMap.at("q"), NameToVarUMap.at("t"),
				NameToVarUMap.at("v") };
			const std::vector<std::reference_wrapper<csp::Variable<std::string>>> graphProbAssignedVarsVec = graphColoringProb.getAssignedVariables();
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> graphProbAssignedVarsUset(graphProbAssignedVarsVec.cbegin(),
				graphProbAssignedVarsVec.cend());
			Assert::IsTrue(assignedVars == graphProbAssignedVarsUset);
		}

		TEST_METHOD(TestGetUnassignedVariables)
		{
			NameToVarUMap.at("q").assignByValue("Red");
			NameToVarUMap.at("t").assignByValue("Blue");
			NameToVarUMap.at("v").assignByValue("Red");
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> unassignedVars{ NameToVarUMap.at("nt"), NameToVarUMap.at("nsw"),
				NameToVarUMap.at("sa"), NameToVarUMap.at("wa") };
			const std::vector<std::reference_wrapper<csp::Variable<std::string>>> graphProbUnassignedVarsVec = graphColoringProb.getUnassignedVariables();
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> graphProbUnassignedVarsUset(graphProbUnassignedVarsVec.cbegin(),
				graphProbUnassignedVarsVec.cend());
			Assert::IsTrue(unassignedVars == graphProbUnassignedVarsUset);
		}

		TEST_METHOD(TestGetNeighbors)
		{
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> saNeighbors{ NameToVarUMap.at("nt"), NameToVarUMap.at("q"),
			NameToVarUMap.at("nsw"), NameToVarUMap.at("v"), NameToVarUMap.at("wa") };
			const std::vector<std::reference_wrapper<csp::Variable<std::string>>> saNeighborsProbVec = graphColoringProb.getNeighbors(NameToVarUMap.at("sa"));
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> saNeighborsProbUset(saNeighborsProbVec.cbegin(),
				saNeighborsProbVec.cend());
			Assert::IsTrue(saNeighbors == saNeighborsProbUset);

			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> vNeighbors{ NameToVarUMap.at("sa"), NameToVarUMap.at("nsw") };
			const std::vector<std::reference_wrapper<csp::Variable<std::string>>> vNeighborsProbVec = graphColoringProb.getNeighbors(NameToVarUMap.at("v"));
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> vNeighborsProbUset(vNeighborsProbVec.cbegin(),
				vNeighborsProbVec.cend());
			Assert::IsTrue(vNeighbors == vNeighborsProbUset);
		}

		TEST_METHOD(TestGetAssignedNeighbors)
		{
			NameToVarUMap.at("wa").assignByValue("Red");
			NameToVarUMap.at("q").assignByValue("Blue");
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> saAssignedNeighbors{ NameToVarUMap.at("q"),
				NameToVarUMap.at("wa") };
			const std::vector<std::reference_wrapper<csp::Variable<std::string>>> saAssignedNeighborsProbVec = graphColoringProb.getAssignedNeighbors(NameToVarUMap.at("sa"));
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> saAssignedNeighborsProbUset(saAssignedNeighborsProbVec.cbegin(),
				saAssignedNeighborsProbVec.cend());
			Assert::IsTrue(saAssignedNeighbors == saAssignedNeighborsProbUset);
		}

		TEST_METHOD(TestGetUnassignedNeighbors)
		{
			NameToVarUMap.at("wa").assignByValue("Red");
			NameToVarUMap.at("q").assignByValue("Blue");
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> saUnassignedNeighbors{ NameToVarUMap.at("nt"),
				NameToVarUMap.at("nsw"),  NameToVarUMap.at("v") };
			const std::vector<std::reference_wrapper<csp::Variable<std::string>>> saUnassignedNeighborsProbVec = graphColoringProb.getUnassignedNeighbors(NameToVarUMap.at("sa"));
			const std::unordered_set<std::reference_wrapper<csp::Variable<std::string>>> saUnassignedNeighborsProbUset(saUnassignedNeighborsProbVec.cbegin(),
				saUnassignedNeighborsProbVec.cend());
			Assert::IsTrue(saUnassignedNeighbors == saUnassignedNeighborsProbUset);
		}

		TEST_METHOD(TestGetConsistentConstraints)
		{
			NameToVarUMap.at("sa").assignByValue("Red");
			NameToVarUMap.at("wa").assignByValue("Blue");
			NameToVarUMap.at("nt").assignByValue("Red");
			NameToVarUMap.at("q").assignByValue("Red");
			NameToVarUMap.at("nsw").assignByValue("Red");
			NameToVarUMap.at("v").assignByValue("Red");

			const std::vector<std::reference_wrapper<csp::Constraint<std::string>>> consistentConstraints{ constraints.at(0), constraints.at(5), constraints.at(9) };
			Assert::IsTrue(consistentConstraints == graphColoringProb.getConsistentConstraints());
		}

		TEST_METHOD(TestGetInconsistentConstraints)
		{
			NameToVarUMap.at("sa").assignByValue("Green");
			NameToVarUMap.at("wa").assignByValue("Green");
			NameToVarUMap.at("nt").assignByValue("Green");

			const std::vector<std::reference_wrapper<csp::Constraint<std::string>>> inconsistentConstraints{ constraints.at(0), constraints.at(1), constraints.at(5) };
			Assert::IsTrue(inconsistentConstraints == graphColoringProb.getInconsistentConstraints());
		}

		TEST_METHOD(TestGetSatisfiedConstraints)
		{
			NameToVarUMap.at("wa").assignByValue("Red");
			NameToVarUMap.at("nt").assignByValue("Green");
			NameToVarUMap.at("sa").assignByValue("Blue");
			NameToVarUMap.at("q").assignByValue("Red");
			NameToVarUMap.at("nsw").assignByValue("Green");
			NameToVarUMap.at("v").assignByValue("Red");
			NameToVarUMap.at("t").assignByValue("Blue");

			Assert::IsTrue(constraints == graphColoringProb.getSatisfiedConstraints());
		}

		TEST_METHOD(TestGetUnsatisfiedConstraints)
		{
			Assert::IsTrue(constraints == graphColoringProb.getUnsatisfiedConstraints());

			for (csp::Variable<std::string>& var : graphColoringProb.getVariables())
			{
				var.assignByValue("Green");
			}

			const std::vector<std::reference_wrapper<csp::Constraint<std::string>>> unsatisfiedConstraints{ constraints.at(0), constraints.at(1),
				constraints.at(2), constraints.at(3), constraints.at(4), constraints.at(5), constraints.at(6), constraints.at(7),
				constraints.at(8) };
			Assert::IsTrue(unsatisfiedConstraints == graphColoringProb.getUnsatisfiedConstraints());
		}

		TEST_METHOD(TestGetConstraintsContainingVariable)
		{
			const std::vector<std::reference_wrapper<csp::Constraint<std::string>>> saConstraints{ constraints.at(0), constraints.at(1),
				constraints.at(2), constraints.at(3), constraints.at(4) };
			Assert::IsTrue(saConstraints == graphColoringProb.getConstraintsContainingVariable(NameToVarUMap.at("sa")));

			const std::vector<std::reference_wrapper<csp::Constraint<std::string>>> tConstraints{ constraints.at(9) };
			Assert::IsTrue(tConstraints == graphColoringProb.getConstraintsContainingVariable(NameToVarUMap.at("t")));

			const std::vector<std::reference_wrapper<csp::Constraint<std::string>>> vConstraints{ constraints.at(4), constraints.at(8) };
			Assert::IsTrue(vConstraints == graphColoringProb.getConstraintsContainingVariable(NameToVarUMap.at("v")));
		}

		TEST_METHOD(TestGetConsistentDomain)
		{
			NameToVarUMap.at("wa").assignByValue("Red");
			const std::vector<std::string> consistentDomain1{ "Blue", "Green" };
			Assert::IsTrue(consistentDomain1 == graphColoringProb.getConsistentDomain(NameToVarUMap.at("sa")));
			Assert::IsTrue(consistentDomain1 == graphColoringProb.getConsistentDomain(NameToVarUMap.at("nt")));

			graphColoringProb.unassignAllVariables();
			NameToVarUMap.at("wa").assignByValue("Red");
			NameToVarUMap.at("nt").assignByValue("Green");
			const std::vector<std::string> consistentDomain2{ "Blue" };
			const auto res = graphColoringProb.getConsistentDomain(NameToVarUMap.at("sa"));
			Assert::IsTrue(consistentDomain2 == graphColoringProb.getConsistentDomain(NameToVarUMap.at("sa")));
		}

		// CSPDO: fix and rewrite this test
		TEST_METHOD(TestGetCurrentAssignment)
		{
			NameToVarUMap.at("wa").assignByValue("Red");
			NameToVarUMap.at("nt").assignByValue("Green");
			NameToVarUMap.at("sa").assignByValue("Blue");
			NameToVarUMap.at("q").assignByValue("Red");
			NameToVarUMap.at("nsw").assignByValue("Green");
			NameToVarUMap.at("v").assignByValue("Red");
			NameToVarUMap.at("t").assignByValue("Blue");

			const std::unordered_map<std::reference_wrapper<csp::Variable<std::string>>, size_t> currAssignment = graphColoringProb.getCurrentAssignment();


			for (const std::pair<std::reference_wrapper<csp::Variable<std::string>>, size_t> varToAssignmentIdx : currAssignment)
			{
				for (const std::pair<std::string, csp::Variable<std::string>>& nameToVar : NameToVarUMap)
				{
					if (varToAssignmentIdx.first.get() == nameToVar.second)		//	CSPDO: here is a bug, it never enters this if.
					{
						if (nameToVar.first == "wa" || nameToVar.first == "q" || nameToVar.first == "v")
						{
							Assert::IsTrue(nameToVar.second.getValue() == "Red");
						}
						else if (nameToVar.first == "nt" || nameToVar.first == "nsw")
						{
							Assert::IsTrue(nameToVar.second.getValue() == "Green");
						}
						else if (nameToVar.first == "sa" || nameToVar.first == "t")
						{
							Assert::IsTrue(nameToVar.second.getValue() == "Blue");
						}
					}
				}
			}
		}

		TEST_METHOD(TestToStringAndWriteToStreamOperator)
		{
			std::ostringstream outStringStream;
			outStringStream << graphColoringProb;
			Assert::AreEqual(graphColoringProb.toString(), outStringStream.str());
		}

		TEST_METHOD(TestGetNameToSolution)
		{
			// CSPDO: add test 
			//std::unordered_map<std::string, std::string> str = graphColoringProb.GetNameToSolution()();
			Assert::IsTrue(true);
		}


		TEST_METHOD(TestDuplicateConstraintError)
		{
			std::vector<std::reference_wrapper<csp::Constraint<std::string>>> otherConstaints = constraints;
			otherConstaints.emplace_back(constr1);
			Assert::ExpectException<csp::duplicate_constraint_error<std::string>>([&]() -> void
				{ csp::ConstraintProblem<std::string> otherConstraintProblem{ otherConstaints }; });
			try
			{
				csp::ConstraintProblem<std::string> otherConstraintProblem{ otherConstaints };
			}
			catch (const csp::duplicate_constraint_error<std::string>& excep)
			{
				Logger::WriteMessage("In TestDuplicateConstraintError:");
				Logger::WriteMessage(excep.what());
			}
		}
	};
}