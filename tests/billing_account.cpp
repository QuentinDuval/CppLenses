#include "billing_account.h"


std::ostream& operator<< (std::ostream& os, address const& a)
{
   return os
      << "{ road = " << a.m_road
      << ", city = " << a.m_city
      << ", state = " << a.m_state
      << " }";
}


std::ostream& operator<< (std::ostream& os, legal_entity const& le)
{
   return os
      << "{ id = " << le.m_id
      << ", description = " << le.m_description
      << " }";
}

std::ostream& operator<< (std::ostream& os, billing_account const& ba)
{
   return os
      << "{ address = " << ba.m_address
      << ", owner = " << ba.m_owner
      << " }";
}