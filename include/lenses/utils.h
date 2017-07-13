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
}

#endif //CPPLENSES_UTILS_H
