//
// Created by Duval Quentin on 09/07/2017.
//

#ifndef CPPLENSES_LENSES_H
#define CPPLENSES_LENSES_H

namespace lenses
{

/**
 * Type declarations expected from a lens
 * @tparam tag
 * @tparam whole
 * @tparam part
 */
template<class tag, class whole, class part>
struct lens_t
{
   using whole_type = whole;
   using part_type = part;
};

template<class whole, class part>
using lens = lens_t<struct lens_tag, whole, part>;

template<class whole, class part>
using traversal = lens_t<struct traversal_tag, whole, part>;


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


/**
 * Composition of two lenses => Yields another range
 * @tparam OuterLens
 * @tparam InnerLens
 */
template<class OuterLens, class InnerLens>
struct dot_lens
{
   using whole_type = typename OuterLens::whole_type;
   using part_type = typename InnerLens::part_type;

   dot_lens(OuterLens outer, InnerLens inner) : m_outer(outer), m_inner(inner)
   {}

   OuterLens m_outer;
   InnerLens m_inner;

   part_type const& operator()(whole_type const& w) const
   {
      return m_inner(m_outer(w));
   }

   template<class OverPart>
   whole_type operator()(whole_type const& w, OverPart&& f) const
   {
      return m_outer(w, [&](auto const& intermediary)
      {
         return m_inner(intermediary, f);
      });
   }
};

template<class OuterLens, class InnerLens>
dot_lens<OuterLens, InnerLens> dot(OuterLens const& o, InnerLens const& i)
{
   return dot_lens<OuterLens, InnerLens>(o, i);
}

}

#endif //CPPLENSES_LENSES_H
