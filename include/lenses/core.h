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
struct optic
{
   using whole_type = whole;
   using part_type = part;
   using tag_type = tag;
};

struct lens_tag {};
struct traversal_tag {};

template<class whole, class part>
using lens = optic<lens_tag, whole, part>;

template<class whole, class part>
using traversal = optic<traversal_tag, whole, part>;


/**
 * Composition of two lenses => Yields another range
 * @tparam OuterLens
 * @tparam InnerLens
 */
namespace details
{
template<class OuterLens, class InnerLens>
class dot_lens_lens
   : public lens<typename OuterLens::whole_type, typename InnerLens::part_type>
{
   using whole = typename OuterLens::whole_type;
   using part = typename InnerLens::part_type;
   OuterLens m_outer;
   InnerLens m_inner;

public:
   dot_lens_lens(OuterLens outer, InnerLens inner) : m_outer(outer), m_inner(inner)
   {}

   part const& view(whole const& w) const
   {
      return m_inner.view(m_outer.view(w));
   }

   template<class OverPart>
   whole over(whole const& w, OverPart&& f) const
   {
      return m_outer.over(w, [&](auto const& intermediary)
      {
         return m_inner.over(intermediary, f);
      });
   }
};

template<class OuterLens, class InnerLens>
class dot_lens_traversal
   : public traversal<typename OuterLens::whole_type, typename InnerLens::part_type>
{
   using whole = typename OuterLens::whole_type;
   using part = typename InnerLens::part_type;
   OuterLens m_outer;
   InnerLens m_inner;

public:
   dot_lens_traversal(OuterLens outer, InnerLens inner) : m_outer(outer), m_inner(inner)
   {}

   std::vector<part> view(whole const& w) const
   {
      return m_inner.view(m_outer.view(w));
   }


   template<class OverPart>
   whole over(whole const& w, OverPart&& f) const
   {
      return m_outer.over(w, [&](auto const& intermediary)
      {
         return m_inner.over(intermediary, f);
      });
   }
};

template<class OuterLens, class InnerLens>
class dot_traversal_traversal
   : public traversal<typename OuterLens::whole_type, typename InnerLens::part_type>
{
   using whole = typename OuterLens::whole_type;
   using part = typename InnerLens::part_type;
   OuterLens m_outer;
   InnerLens m_inner;

public:
   dot_traversal_traversal(OuterLens outer, InnerLens inner) : m_outer(outer), m_inner(inner)
   {}

   std::vector<part> view(whole const& w) const
   {
      std::vector<part> out;
      for (auto const& o: m_outer.view(w))
         for (auto const& i: m_inner.view(o))
            out.push_back(i);
      return out;
   }


   template<class OverPart>
   whole over(whole const& w, OverPart&& f) const
   {
      return m_outer.over(w, [&](auto const& intermediary)
      {
         return m_inner.over(intermediary, f);
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
   return l.view(w);
};

template <class whole, class lens, class over>
auto update_in(whole const& w, lens const& l, over&& f)
{
   return l.over(w, f);
};
}

#endif //CPPLENSES_CORE_H
