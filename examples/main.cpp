#include <iostream>

namespace not_composable
{
using uuid = std::string;

struct address
{
   const std::string m_road;
   const std::string m_city;
   const std::string m_state;
};

struct legal_entity
{
   const uuid m_id;
   const std::string m_description;
};

struct billing_account
{
   const address m_address;
   const legal_entity m_owner;
};

billing_account set_address(billing_account const& acc, address const& address)
{
   return billing_account{address, acc.m_owner};
}

address set_road(address const& old, std::string const& road)
{
   return address {road, old.m_city, old.m_state};
}

std::string add_exclamation_mark(std::string const& s)
{
   return s + "!";
}

void test()
{
   auto old_account = billing_account{
      address{"Road", "City", "State"},
      legal_entity {"UUID", "Description"}};

   auto new_account = set_address(
      old_account,
      set_road(
         old_account.m_address,
         add_exclamation_mark(old_account.m_address.m_road)
      ));

   std::cout << new_account.m_address.m_road << std::endl;
}
}

int main()
{
   not_composable::test();
   return 0;
}