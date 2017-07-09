#ifndef CPPLENSES_BILLING_ACCOUNT_H
#define CPPLENSES_BILLING_ACCOUNT_H

#include <string>
#include <sstream>


// -----------------------------------------------------------------------------
// Example of data structures
// -----------------------------------------------------------------------------

using uuid = std::string;

struct address
{
	std::string m_road;
	std::string m_city;
	std::string m_state;
};

struct legal_entity
{
	uuid m_id;
	std::string m_description;
};

struct billing_account
{
	address m_address;
	legal_entity m_owner;
};


// -----------------------------------------------------------------------------
// Helpers for tests
// -----------------------------------------------------------------------------

std::ostream& operator<< (std::ostream& os, address const& a);
std::ostream& operator<< (std::ostream& os, legal_entity const& le);
std::ostream& operator<< (std::ostream& os, billing_account const& ba);


#endif //CPPLENSES_BILLING_ACCOUNT_H
