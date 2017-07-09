#include <gtest/gtest.h>

#include "billing_account.h"
#include "lenses/lenses.h"


// -----------------------------------------------------------------------------
// Test Fixture
// -----------------------------------------------------------------------------

struct address_lens : lenses::field_lens_t<billing_account, address>
{
   address_lens() : make_field_lens(&billing_account::m_address)
   {}
};

struct road_lens : lenses::field_lens_t<address, std::string>
{
   road_lens() : make_field_lens(&address::m_road)
   {}
};

struct LensesShould : ::testing::Test
{
   address sample_address()
   {
      return {"Road", "City", "State"};
   }

   billing_account sample_account()
   {
      return {sample_address(), legal_entity {"UUID", "Description"}};
   }
};

static std::string add_exclamation_mark(std::string const& s)
{
   return s + "!";
}

// -----------------------------------------------------------------------------
// Tests
// -----------------------------------------------------------------------------

TEST_F(LensesShould, read_direct_field)
{
   EXPECT_EQ("Road", road_lens()(sample_address()));
}

TEST_F(LensesShould, compose_read)
{
   auto billing_account_road = dot(address_lens(), road_lens());
   EXPECT_EQ("Road", billing_account_road(sample_account()));
}

TEST_F(LensesShould, allow_direct_mutation)
{
   auto new_address = road_lens()(sample_address(), add_exclamation_mark);
   EXPECT_EQ("Road!", road_lens()(new_address));
}

TEST_F(LensesShould, compose_into_nested_mutations)
{
   auto billing_account_road = dot(address_lens(), road_lens());
   auto new_billing_account = billing_account_road(sample_account(), add_exclamation_mark);
   EXPECT_EQ("Road!", billing_account_road(new_billing_account));
}
