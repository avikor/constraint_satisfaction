#pragma once

#include "pch.h"
#include "initial_utilities.h"


namespace csp
{
	class template_argument_T_not_writable_to_cout_error;
	template<typename T> class unassigned_value_extraction_error;
	template<typename T> class over_assignment_error;
	template<typename T> class assignment_idx_out_of_range_error;
	template<typename T> class uncontained_value_error;
	template<typename T> class domain_alteration_error;
	
	/*
	Assumptions on T:
	1. T has a copy constructor (used in Variable<T>::getValue()).
	2. hash<T> exists for T (used in ConstraintProblem<T>::getConsistentDomain(Variable<T>& var)).
	3. T is writable to ostream (used in friend ostream& operator<<(ostream& os, const Variable<T>& variable))
	Note: Assumptions 1 and 2 always hold by virtue of demanding unordered_set<T> in Variable<T>'s constructor.
		 Assumption 3 hold by the static method Variable<T>::initDomain(const unordered_set<T>& domain).
	*/
	template <typename T>
	class Variable final
	{
	public:
		Variable<T>() = delete;
		
		Variable<T>(const std::unordered_set<T>& domain) :
			m_optCompare{ init_compare() },
			m_vecDomain{ init_domain(domain) },
			m_size_tValueIdx{ UNASSIGNED }
		{ }

		~Variable<T>() = default;

		Variable<T>(const Variable<T>& otherVar) :
			m_optCompare{ otherVar.m_optCompare },
			m_vecDomain{ otherVar.m_vecDomain }, 
			m_size_tValueIdx{ otherVar.m_size_tValueIdx }
		{ }

		Variable<T>& operator=(const Variable<T>& otherVar)
		{
			return *this = Variable<T>(otherVar);
		}

		Variable<T>(Variable<T>&& otherVar) noexcept : 
			m_optCompare{ std::move(otherVar.m_optCompare) },
			m_vecDomain{ std::move(otherVar.m_vecDomain) },
			m_size_tValueIdx{ otherVar.m_size_tValueIdx }
		{ }

		Variable<T>& operator=(Variable<T>&& otherVar) noexcept
		{
			std::swap(m_optCompare, otherVar.m_optCompare);
			std::swap(m_vecDomain, otherVar.m_vecDomain);
			std::swap(m_size_tValueIdx, otherVar.m_size_tValueIdx);
			return *this;
		}

		constexpr bool isDomainSorted() { return m_optCompare.has_value(); }

		constexpr bool isAssigned() const noexcept { return m_size_tValueIdx != UNASSIGNED; }

		constexpr T getValue() const
		{
			if (!this->isAssigned())
			{
				throw unassigned_value_extraction_error<T>{ *this };
			}
			return m_vecDomain[m_size_tValueIdx];
		}

		constexpr size_t getAssignmentIdx() const noexcept { return m_size_tValueIdx; }

		void unassign() noexcept { m_size_tValueIdx = UNASSIGNED; }

		void assignByIdx(size_t assignmentIdx)
		{
			if (m_vecDomain.size() <= assignmentIdx)
			{
				throw assignment_idx_out_of_range_error<T>{ *this, std::to_string(assignmentIdx) };
			}
			if (this->isAssigned())
			{
				throw over_assignment_error<T>{ *this };
			}

			m_size_tValueIdx = assignmentIdx;
		}

		void assignByValue(T val)
		{
			if (this->isAssigned())
			{
				throw over_assignment_error<T>{ *this };
			}
			
			if (m_optCompare)
			{
				m_size_tValueIdx = this->get_assignment_idx_for_value_in_sorted_domain(val);
			}
			else
			{
				m_size_tValueIdx = this->get_assignment_idx_for_value_in_unsorted_domain(val);
			}
		}

		constexpr size_t getAssignmentIdxOfValue(T value) const
		{
			if (m_optCompare)
			{
				return this->get_assignment_idx_for_value_in_sorted_domain(value);
			}
			else
			{
				return this->get_assignment_idx_for_value_in_unsorted_domain(value);
			}
		}

		void assignWithRandomlySelectedValue()
		{
			if (this->isAssigned())
			{
				throw over_assignment_error<T>{ *this };
			}

			std::random_device randomDevice;
			std::default_random_engine defaultRandomEngine{ randomDevice() };
			std::uniform_int_distribution<size_t> zeroToDomainLenDistribution(0, m_vecDomain.size() - 1);
			m_size_tValueIdx = zeroToDomainLenDistribution(defaultRandomEngine);
		}

		const std::vector<T>& getDomain() const noexcept
		{
			return m_vecDomain;
		}
		
		void setDomain(const std::vector<T>& domain, bool checkUniqueness = false)
		{
			if(!checkUniqueness)
				m_vecDomain = domain;
			else
			{
				std::unordered_set<T> elems { domain.cbegin(), domain.cend() };
				if(elems.size() == domain.size())
					m_vecDomain = domain;
			}
		}

		bool setSubsetDomain(const std::vector<T>& vecSubsetDomain, bool subsetDomainIsSorted = true)
		{
			bool wasDomainShortened = false;
			if (this->isAssigned())
			{
				throw domain_alteration_error<T>{ *this };
			}

			if (m_optCompare)
			{
				if (!subsetDomainIsSorted)
				{
					std::vector<T>& noConstSubsetDomain = const_cast<std::vector<T>&>(vecSubsetDomain);
					std::sort(noConstSubsetDomain.begin(), noConstSubsetDomain.end());
				}

				if (std::includes(m_vecDomain.cbegin(), m_vecDomain.cend(), vecSubsetDomain.cbegin(), vecSubsetDomain.cend()))
				{
					m_vecDomain = vecSubsetDomain;
					wasDomainShortened = true;
				}
			}
			else
			{
				wasDomainShortened = this->set_subset_domain_for_unsorted_domain(vecSubsetDomain);
			}

			if (wasDomainShortened)
			{
				m_size_tValueIdx = UNASSIGNED;
			}

			return wasDomainShortened;
		}

		void removeFromDomainByIdx(size_t idx)
		{
			if (this->isAssigned())
			{
				throw domain_alteration_error<T>(*this);
			}
			m_vecDomain.erase(m_vecDomain.begin() + idx);
			m_size_tValueIdx = UNASSIGNED;
		}

		void removeFromDomainByValue(T val)
		{
			if (this->isAssigned())
			{
				throw domain_alteration_error<T>(*this);
			}
			auto it = std::find(m_vecDomain.cbegin(), m_vecDomain.cend(), val);
			m_vecDomain.erase(it);
			m_size_tValueIdx = UNASSIGNED;
		}

		friend std::ostream& operator<<(std::ostream& os, const Variable<T>& variable) noexcept
		{
			os << "(variable's value: ";
			variable.writeAssignmentToOutStream(os);

			os << ", variable's domain: ";
			char sep = '\0';
			for (T elem : variable.m_vecDomain)
			{
				os << sep << elem;
				if (!sep)
				{
					sep = ' ';
				}
			}
			os << ')';
			return os;
		}

		constexpr std::string toString() const noexcept
		{
			std::ostringstream outStringStream;
			outStringStream << *this;
			return outStringStream.str();
		}

		void writeAssignmentToOutStream(std::ostream& outStream) const noexcept
		{
			if (this->isAssigned())
			{
				outStream << this->getValue();
			}
			else
			{
				outStream << "unassigned";
			}
		}

		friend bool operator==(const Variable<T>& left, const Variable<T>& right) noexcept
		{
			return &(left) == &(right);
		}

		friend bool operator!=(const Variable<T>& left, const Variable<T>& right) noexcept
		{
			return !(left == right);
		}

		static std::unordered_map<std::string, Variable<T>> constructFromNamesToEqualDomain(const std::unordered_set<std::string>& names,
			const std::unordered_set<T>& domain) noexcept
		{
			std::unordered_map<std::string, Variable<T>> nameToVarMap;
			nameToVarMap.reserve(names.size());
			for (const std::string& varName : names)
			{
				nameToVarMap.emplace(varName, domain);
			}
			return nameToVarMap;
		}

		static std::unordered_map<std::string, Ref<Variable<T>>> constructFromNamesToEqualDomainPutInVec(const std::unordered_set<std::string>& names,
			const std::unordered_set<T>& domain, std::vector<Variable<T>>& variables)
		{
			variables.reserve(names.size());
			for (size_t i = 0; i < names.size(); ++i)
			{
				variables.emplace_back(domain);
			}

			std::unordered_map<std::string, Ref<Variable<T>>> nameToVarRefMap;
			nameToVarRefMap.reserve(names.size());
			size_t i = 0;
			for (const std::string& varName : names)
			{
				nameToVarRefMap.emplace(varName, variables[i]);
				++i;
			}
			
			return nameToVarRefMap;
		}

		static std::unordered_map<std::string, Ref<Variable<T>>> constructFromNamesToDomainsPutInVec(
			std::unordered_map<std::string, std::unordered_set<T>> NameToDomainMap, std::vector<Variable<T>>& variables)
		{
			variables.reserve(NameToDomainMap.size());
			
			std::unordered_map<std::string, Ref<Variable<T>>> nameToVarRefMap;
			nameToVarRefMap.reserve(NameToDomainMap.size());

			for (const std::pair<std::string, std::unordered_set<T>>& nameToDomain : NameToDomainMap)
			{
				variables.emplace_back(nameToDomain.second);
				nameToVarRefMap.emplace(nameToDomain.first, variables.back());
			}

			return nameToVarRefMap;
		}


		private:
			static std::optional<std::function<constexpr bool(T left, T right)>> init_compare()
			{
				// CSPDO: test it in cspTests
				static_assert(__is_to_stream_writable<std::ostream, T>::value, "T must be writable to std::cout.");

				return std::less<T>();

				//// CSPDO: test it in cspTests
				//std::optional<std::function<bool(T left, T right)>> optCompare;
				//if constexpr (std::is_same<__T_less_than_operator_return_type<T>, bool>::value)
				//{
				//	optCompare = std::less<T>();
				//}

				//return optCompare;
			}

			std::vector<T> init_domain(const std::unordered_set<T>& domain) noexcept
			{
				std::vector<T> vecDomain{ domain.cbegin(), domain.cend() };

				if (m_optCompare)
				{
					std::sort(vecDomain.begin(), vecDomain.end(), *m_optCompare);
				}

				return vecDomain;
			}

			size_t get_assignment_idx_for_value_in_sorted_domain(T value) const
			{
				const auto itToBeginDomain = m_vecDomain.cbegin();
				const auto itToValPosition = std::lower_bound(itToBeginDomain, m_vecDomain.cend(), value);
				if (*itToValPosition != value)
				{
					throw uncontained_value_error<T>{ *this, value};
				}
				else
				{
					return itToValPosition - itToBeginDomain;
				}
			}

			size_t get_assignment_idx_for_value_in_unsorted_domain(T value) const
			{
				size_t idx = 0;
				bool isValFound = false;
				for (; idx < m_vecDomain.size(); ++idx)
				{
					if (m_vecDomain[idx] == value)
					{
						isValFound = true;
						break;
					}
				}

				if (!isValFound)
				{
					throw uncontained_value_error<T>{ *this, value };
				}
				else
				{
					return idx;
				}
			}

			bool set_subset_domain_for_unsorted_domain(const std::vector<T>& vecSubsetDomain)
			{
				bool wasDomainShortened = false;
				if (m_vecDomain.size() <= vecSubsetDomain.size())
				{
					return wasDomainShortened;
				}

				std::unordered_set<T> usetDomain{ m_vecDomain.cbegin(), m_vecDomain.cend() };
				for (T value : vecSubsetDomain)
				{
					if (!usetDomain.count(value))
					{
						return wasDomainShortened;
					}
				}

				m_vecDomain = vecSubsetDomain;
				m_size_tValueIdx = UNASSIGNED;
				wasDomainShortened = true;
				return wasDomainShortened;
			}


			std::optional<std::function<constexpr bool(T left, T right)>> m_optCompare;
			std::vector<T> m_vecDomain;
			size_t m_size_tValueIdx;
	};


	class template_argument_T_not_writable_to_cout_error : public std::invalid_argument
	{
	public:
		template_argument_T_not_writable_to_cout_error() :
			invalid_argument{ "Given template argument T must be writable to std::cout." }
		{ }
	};

	template<typename T>
	class unassigned_value_extraction_error : public std::logic_error
	{
	public:
		unassigned_value_extraction_error(const Variable<T>& var) :
			logic_error{ "Extracting value of unassigned variable: " + var.toString() +
				", variable must be assigned for value extraction." }
		{ }
	};


	template<typename T>
	class over_assignment_error : public std::logic_error
	{
	public:
		over_assignment_error(const Variable<T>& var) : 
			logic_error{ "Over-assignment of an assigned variable: " + var.toString() + 
			", variable must be unassigned before assignment." }
		{ }
	};

	template<typename T>
	class assignment_idx_out_of_range_error : public std::out_of_range
	{
	public:
		assignment_idx_out_of_range_error(const Variable<T>& var, const std::string& assignmentIdx) :
			out_of_range{ "Assignment index: " + assignmentIdx + " of variable: " + var.toString() +
			" , is bigger than variable's domain size." }
		{ }
	};

	template<typename T>
	class domain_alteration_error : public std::logic_error
	{
	public:
		domain_alteration_error(const Variable<T>& var) : 
			std::logic_error{ "Tried to alter domain of assigned variable: " + var.toString() +
			", variable must be unassigned before domain could be altered." }
		{ }
	};

	template<typename T>
	class uncontained_value_error : public std::domain_error
	{
	private:
		static const std::string getValueStr(T value)
		{
			std::ostringstream outStringStream;
			outStringStream << value;
			return outStringStream.str();
		}

	public:
		uncontained_value_error(const Variable<T>& var, T value) :
			std::domain_error{ "Cannot assign variable: " + var.toString()
			+ " with value: " + getValueStr(value) + " since it is not contained in variable's domain." }
		{ }
	};
}


namespace std
{
	template <typename T>
	struct std::hash<typename csp::Variable<T>>
	{
		size_t operator()(const csp::Variable<T>& var) const
		{
			csp::Variable<T>* pVar = const_cast<csp::Variable<T>*>(&(var));
			std::hash<csp::Variable<T>*> varPtrHasher;
			return varPtrHasher(pVar);
		}
	};
}

namespace std
{
	template <typename T>
	struct std::hash<typename std::reference_wrapper<csp::Variable<T>>>
	{
		size_t operator()(const std::reference_wrapper<csp::Variable<T>>& varRef) const
		{
			std::hash<csp::Variable<T>> varHasher;
			return varHasher(varRef.get());
		}
	};
}