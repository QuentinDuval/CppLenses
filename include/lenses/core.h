//
// Created by Duval Quentin on 12/07/2017.
//

#ifndef CPPLENSES_CORE_H
#define CPPLENSES_CORE_H

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
   using tag_type = tag;
};

struct lens_tag {};
struct traversal_tag {};

template<class whole, class part>
using lens = lens_t<lens_tag, whole, part>;

template<class whole, class part>
using traversal = lens_t<traversal_tag, whole, part>;


/**
 * Composition of two lenses => Yields another range
 * @tparam OuterLens
 * @tparam InnerLens
 */
namespace details
{
template<class OuterLens, class InnerLens>
struct dot_lens_lens
   : lens<typename OuterLens::whole_type, typename InnerLens::part_type>
{
   using whole_type = typename OuterLens::whole_type;
   using part_type = typename InnerLens::part_type;

   dot_lens_lens(OuterLens outer, InnerLens inner) : m_outer(outer), m_inner(inner)
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
struct dot_lens_traversal
   : traversal<typename OuterLens::whole_type, typename InnerLens::part_type>
{
   using whole_type = typename OuterLens::whole_type;
   using part_type = typename InnerLens::part_type;

   dot_lens_traversal(OuterLens outer, InnerLens inner) : m_outer(outer), m_inner(inner)
   {}

   OuterLens m_outer;
   InnerLens m_inner;

   std::vector<part_type> operator()(whole_type const& w) const
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
struct dot_traversal_traversal
   : traversal<typename OuterLens::whole_type, typename InnerLens::part_type>
{
   using whole_type = typename OuterLens::whole_type;
   using part_type = typename InnerLens::part_type;

   dot_traversal_traversal(OuterLens outer, InnerLens inner) : m_outer(outer), m_inner(inner)
   {}

   OuterLens m_outer;
   InnerLens m_inner;

   std::vector<part_type> operator()(whole_type const& w) const
   {
      std::vector<part_type> out;
      for (auto const& o: m_outer(w))
         for (auto const& i: m_inner(o))
            out.push_back(i);
      return out;
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
dot_lens_lens<OuterLens, InnerLens> dot_impl(OuterLens const& o, InnerLens const& i, lens_tag, lens_tag)
{
   return {o, i};
}

template<class OuterLens, class InnerLens>
dot_lens_traversal<OuterLens, InnerLens> dot_impl(OuterLens const& o, InnerLens const& i, lens_tag, traversal_tag)
{
   return {o, i};
}

template<class OuterLens, class InnerLens>
dot_traversal_traversal<OuterLens, InnerLens> dot_impl(OuterLens const& o, InnerLens const& i, traversal_tag, traversal_tag)
{
   return {o, i};
}

//TODO - traversal - lens
}

template<class OuterLens, class InnerLens>
auto operator/(OuterLens const& o, InnerLens const& i)
{
   return details::dot_impl(o, i, typename OuterLens::tag_type{}, typename InnerLens::tag_type{});
}


/**
 * Main functions to read, and modify a data structure with a lens
 */
template <class whole, class lens>
auto get_in(whole const& w, lens const& l)
{
   return l(w);
};

template <class whole, class lens, class over>
auto update_in(whole const& w, lens const& l, over&& f)
{
   return l(w, f);
};
}

#endif //CPPLENSES_CORE_H
