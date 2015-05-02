#include "room/sst/parser.h"

std::unique_ptr<room::sst::Space> room::sst::parseSpace(std::unique_ptr<room::lst::Symbol>)
{

}

std::unique_ptr<room::sst::Subject> room::sst::parseSubject(std::unique_ptr<room::lst::Symbol> set, const Aliases &ctx)
{
    static SubjectBuilder builder {
        Set() = [](Set p, Subject &parent){
            return Perspective(p, parent.aliases());
        },
        Atom("like").Set().Atom(">>").Set() = [](Set sbj, Set spc, Subject &parent) {
            const Aliases a = parent.aliases() + parent.perspective().aliases();
            return Extension(parseSubject(sbj, a), parseSpace(spc, a))
        },
        Atom("like").Set().Atom(">>").Set() = [](Set sbj) {
            const Aliases a = parent.aliases() + parent.perspective().aliases();
            return Extension(parseSubject(sbj, a));
        },
        Set().Atom(">>").Set() = [](Set sbj, Set spc) {
            const Aliases a = parent.aliases() + parent.perspective().aliases();
            return Emission(parseSubject(sbj, a), parseSpace(spc, a));
        },
        Atom("->").Set() = [](Set sbj) {
            const Aliases a = parent.aliases() + parent.perspective().aliases();
            return Transformation1(parseSubject(sbj, a));
        },
        Atom("=>").Set() = [](Set sbj) {
            const Aliases a = parent.aliases() + parent.perspective().aliases();
            return Transformation2(parseSubject(sbj, a));
        },
        Any().Atom("=").Any() = [](room::lst::Symbol *alias, room::lst::Symbol *symbol) {
            const Aliases a = parent.aliases() + parent.perspective().aliases();
            return Alias(alias, symbol, a);
        }
    };

    Subject sbj(ctx);

    while (auto part = builder(Stream(set->elements), sbj)) {
        sbj.append(part);
    }
}

std::unique_ptr<room::sst::Perspective> room::sst::parsePerspective(std::unique_ptr<room::lst::Symbol>)
{

}
