//
// Created by Duval Quentin on 12/07/2017.
//

#ifndef CPPLENSES_UTILS_H
#define CPPLENSES_UTILS_H

#include "lenses/core.h"

namespace lenses
{
/**
 * Builds a simple lens based on a field
 * @tparam whole
 * @tparam part
 */
template<class whole, class part, part whole::*member>
class make_field_lens : public lens<whole, part>
{
public:
   part const& view(whole const& ba) const
   {
      return ba.*member;
   }

   template<class OverPart>
   whole over(whole const& ba, OverPart&& f) const
   {
      auto new_ba = ba;
      new_ba.*member = f(view(ba));
      return new_ba;
   }
};

/**
 * Helper macro to define a lens, based on the conventions:
 * - the member must be public and starting with "m_"
 * - the generated lens will be named "name" followed by "_l"
 */
#define MAKE_LENS(name, whole, part) \
   using name##_l = lenses::make_field_lens<whole, part, &whole::m_##name>;
}

#endif //CPPLENSES_UTILS_H
