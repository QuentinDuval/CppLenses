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
template<class whole, class part>
class field_lens_t : public lens<whole, part>
{
public:
   using make_field_lens = field_lens_t<whole, part>;

   field_lens_t(part whole::*field) : m_field(field)
   {}

   part const& operator()(whole const& ba) const
   {
      return ba.*m_field;
   }

   template<class OverPart>
   whole operator()(whole const& ba, OverPart&& f) const
   {
      auto new_ba = ba;
      new_ba.*m_field = f((*this)(ba));
      return new_ba;
   }

private:
   part whole::*m_field;
};
}

#endif //CPPLENSES_UTILS_H
