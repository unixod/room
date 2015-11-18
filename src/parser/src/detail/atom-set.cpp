#include "detail/atom-set.h"

namespace detail = room::parser::detail;

detail::AtomSet::AtomSet(detail::AtomSet&& oth)
noexcept (std::is_nothrow_move_constructible<detail::AtomSet::SetDescription>::value &&
          std::is_nothrow_move_constructible<detail::AtomSet::AtomDescription>::value)
{
    switch(type = oth.type) {
    case Type::Set:
        set = std::move(oth.set);
        break;
    case Type::Atom:
        atom = std::move(oth.atom);
        break;
    }
}

detail::AtomSet::~AtomSet()
{
    switch(type) {
    case Type::Set:
        set.~SetDescription();
        break;
    case Type::Atom:
        atom.~AtomDescription();
        break;
    }
}

detail::AtomSet & detail::AtomSet::operator = (detail::AtomSet&& oth)
noexcept (std::is_nothrow_move_assignable<SetDescription>::value &&
          std::is_nothrow_move_assignable<AtomDescription>::value)
{
    switch(type = oth.type) {
    case Type::Set:
        set = std::move(oth.set);
        break;
    case Type::Atom:
        atom = std::move(oth.atom);
        break;
    }

    return *this;
}

detail::AtomSet & detail::AtomSet::operator = (const detail::AtomSet& oth)
{
    switch(type = oth.type) {
    case Type::Set:
        set = oth.set;
        break;
    case Type::Atom:
        atom = oth.atom;
        break;
    }

    return *this;
}

