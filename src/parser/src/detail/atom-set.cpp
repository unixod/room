#include <stdexcept>
#include "room/common.h"
#include "detail/atom-set.h"

namespace detail = room::parser::detail;

detail::AtomSet::AtomSet(detail::AtomSet&& oth)
noexcept (std::is_nothrow_move_constructible<detail::AtomSet::SetDescription>::value &&
          std::is_nothrow_move_constructible<detail::AtomSet::AtomDescription>::value)
{
    switch(oth.type) {
    case Type::Set:
        new(&set) SetDescription{std::move(oth.set)};
        break;
    case Type::Atom:
        new(&atom) AtomDescription{std::move(oth.atom)};
        break;
    case Type::Undefined:
        throw std::runtime_error {
            ERROR_MSG "Internal error, attempt to initialize from 'Undefined' AtomSet"
        };
        break;
    }

    type = oth.type;
    sibling = std::move(oth.sibling);

    oth.type = Type::Undefined;
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
    case Type::Undefined:
        /*none*/
        break;
    }
}

detail::AtomSet & detail::AtomSet::operator = (detail::AtomSet&& oth)
noexcept (std::is_nothrow_move_assignable<SetDescription>::value &&
          std::is_nothrow_move_assignable<AtomDescription>::value)
{
    switch(oth.type) {
    case Type::Set:
        this->~AtomSet();
        new(&set) SetDescription{std::move(oth.set)};
        break;
    case Type::Atom:
        this->~AtomSet();
        new(&atom) AtomDescription{std::move(oth.atom)};
        break;
    case Type::Undefined:
        throw std::runtime_error {
            ERROR_MSG "Internal error, attempt to assign 'Undefined' AtomSet"
        };
        break;
    }

    type = oth.type;
    sibling = std::move(oth.sibling);

    oth.type = Type::Undefined;

    return *this;
}

