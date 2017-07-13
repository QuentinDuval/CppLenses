//
// Created by Duval Quentin on 12/07/2017.
//

#ifndef CPPLENSES_STRINGS_H
#define CPPLENSES_STRINGS_H

#include "lenses/core.h"

#include <string>
#include <vector>


namespace lenses
{
struct all_characters : lenses::traversal<std::string, char>
{
   std::vector<part_type> view(whole_type const& w) const
   {
      return std::vector<part_type>(w.begin(), w.end());
   }

   template<class OverPart>
   whole_type over(whole_type const& w, OverPart&& f) const
   {
      whole_type out;
      out.reserve(w.size());
      std::transform(w.begin(), w.end(), std::back_inserter(out), f);
      return out;
   }
};
}

#endif //CPPLENSES_STRINGS_H
