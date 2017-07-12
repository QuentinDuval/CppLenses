#include <gtest/gtest.h>

#include "billing_account.h"
#include "lenses/lenses.h"

#include <algorithm>
#include <vector>


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
// Tests (Lens - Lens)
// -----------------------------------------------------------------------------

TEST_F(LensesShould, read_direct_field)
{
   EXPECT_EQ("Road", get_in(sample_address(), road_lens()));
}

TEST_F(LensesShould, compose_read)
{
   EXPECT_EQ("Road", get_in(sample_account(), address_lens(), road_lens()));
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

// -----------------------------------------------------------------------------
// Tests (Lens - Traversal)
// -----------------------------------------------------------------------------

struct all_address_fields : lenses::traversal<address, std::string>
{
   std::vector<part_type> operator()(whole_type const& w) const
   {
      return { w.m_road, w.m_city, w.m_state };
   }

   template<class OverPart>
   whole_type operator()(whole_type const& w, OverPart&& f) const
   {
      //TODO - Other possibility would be to apply f on the vector... more general, more risky
      auto copy = w;
      copy.m_road = f(w.m_road);
      copy.m_city = f(w.m_city);
      copy.m_state = f(w.m_state);
      return copy;
   }
};

TEST_F(LensesShould, compose_lens_with_traversals_for_reads)
{
   auto all_strings = dot(address_lens(), all_address_fields());
   auto result = all_strings(sample_account());
   ASSERT_EQ(3, result.size());
   EXPECT_EQ("Road", result[0]);
   EXPECT_EQ("City", result[1]);
   EXPECT_EQ("State", result[2]);
}

TEST_F(LensesShould, compose_lens_with_traversals_for_updates)
{
   auto all_strings = dot(address_lens(), all_address_fields());
   auto new_account = all_strings(sample_account(), add_exclamation_mark);
   auto result = all_strings(new_account);
   ASSERT_EQ(3, result.size());
   EXPECT_EQ("Road!", result[0]);
   EXPECT_EQ("City!", result[1]);
   EXPECT_EQ("State!", result[2]);
}

// -----------------------------------------------------------------------------
// Tests (Traversal - Traversal)
// -----------------------------------------------------------------------------

TEST_F(LensesShould, compose_traversal_with_traversals_for_reads)
{
   auto all_fields_characters = dot(all_address_fields(), lenses::all_characters());
   auto result = all_fields_characters(sample_address());
   ASSERT_EQ(13, result.size());
}

TEST_F(LensesShould, compose_traversal_with_traversals_for_updates)
{
   auto all_fields_characters = dot(all_address_fields(), lenses::all_characters());
   auto new_address = all_fields_characters(sample_address(), [](char c) { return std::tolower(c); });
   auto result = all_address_fields()(new_address);
   ASSERT_EQ(3, result.size());
   EXPECT_EQ("road", result[0]);
   EXPECT_EQ("city", result[1]);
   EXPECT_EQ("state", result[2]);
}

// -----------------------------------------------------------------------------
// Tests (Path of length 3)
// -----------------------------------------------------------------------------

TEST_F(LensesShould, compose_lens_with_traversal_with_traversals_for_updates)
{
   auto all_strings = dot(address_lens(), all_address_fields());
   auto all_address_chars = dot(all_strings, lenses::all_characters());
   auto new_address = all_address_chars(sample_account(), [](char c) { return std::tolower(c); });
   auto result = all_strings(new_address);
   ASSERT_EQ(3, result.size());
   EXPECT_EQ("road", result[0]);
   EXPECT_EQ("city", result[1]);
   EXPECT_EQ("state", result[2]);
}
