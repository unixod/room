/* Generated by re2c 0.13.7.5 */
#line 1 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
#ifndef ROOM_LEXER_PRIVATE_H
#define ROOM_LEXER_PRIVATE_H

#include <algorithm>
#include <functional>
#include <vector>
#include <cstdint>
#include "lexer.re2c.conditions.h"
#include "room/lexer/token.h"
#include "room/lexer.h"

namespace lexer = room::lexer;

namespace room {
namespace lexer {
namespace detail {

struct Re2cLexerState {
    typedef char CharType;
    YYCONDTYPE cond;

    // when refilling, piece between
    // the lexemeStart and lexemeEnd
    // may be stored in temporary
    // storage as needed, and in final
    // concatenated with tail of lexeme
    CharType *lexemeStart;

    // when refilling, the following
    // pointers should be adjusted
    CharType *lexemeEnd;
    CharType *inputBufferEnd;
    CharType *ctxMarker;
    CharType *marker;
};

Token::Category nextToken(Re2cLexerState &state, std::function<void(std::size_t)> refill) {
#   define GOTO_COND(condition_name) goto cond_##condition_name
#   define YYGETCONDITION() state.cond
#   define YYSETCONDITION(x) { state.cond = (x); }

    // in reffiling function we use algorithm which finds
    // the closest to the state.lexemeStart pointer
    state.lexemeStart = state.lexemeEnd;
    state.ctxMarker = state.inputBufferEnd;
    state.marker = state.inputBufferEnd;


#line 52 "lexer_p.h"
{
    Re2cLexerState::CharType yych;
    switch (YYGETCONDITION()) {
    case COMMENT: goto cond_COMMENT;
    case REGULAR_SPACE: goto cond_REGULAR_SPACE;
    case SINGULAR_SPACE: goto cond_SINGULAR_SPACE;
    }
/* *********************************** */
cond_COMMENT:
    if (state.inputBufferEnd <= state.lexemeEnd) refill(1);
    yych = *state.lexemeEnd;
    switch (yych) {
    case 0x00:    goto yy3;
    case '"':    goto yy7;
    default:    goto yy5;
    }
yy3:
    ++state.lexemeEnd;
#line 78 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { --state.lexemeEnd; return Token::Category::Error; }
#line 73 "lexer_p.h"
yy5:
    ++state.lexemeEnd;
#line 82 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    goto cond_COMMENT;
#line 78 "lexer_p.h"
yy7:
    ++state.lexemeEnd;
    YYSETCONDITION(REGULAR_SPACE);
#line 81 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    goto cond_REGULAR_SPACE;
#line 84 "lexer_p.h"
/* *********************************** */
cond_REGULAR_SPACE:
    if ((state.inputBufferEnd - state.lexemeEnd) < 2) refill(2);
    yych = *state.lexemeEnd;
    switch (yych) {
    case 0x00:    goto yy11;
    case '\t':
    case ' ':    goto yy15;
    case '\n':    goto yy17;
    case '"':    goto yy18;
    case '\'':    goto yy20;
    case '(':    goto yy22;
    case '\\':    goto yy24;
    case '|':    goto yy25;
    case '}':    goto yy26;
    default:    goto yy13;
    }
yy11:
    ++state.lexemeEnd;
#line 85 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { --state.lexemeEnd; return Token::Category::End; }
#line 106 "lexer_p.h"
yy13:
    ++state.lexemeEnd;
    yych = *state.lexemeEnd;
    goto yy29;
yy14:
#line 93 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::Atom; }
#line 114 "lexer_p.h"
yy15:
    ++state.lexemeEnd;
yy16:
#line 91 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { ++state.lexemeStart; GOTO_COND(REGULAR_SPACE); }
#line 120 "lexer_p.h"
yy17:
    yych = *++state.lexemeEnd;
    switch (yych) {
    case 0x00:
    case '\t':
    case ' ':
    case '(':
    case '|':    goto yy16;
    default:    goto yy29;
    }
yy18:
    ++state.lexemeEnd;
    switch ((yych = *state.lexemeEnd)) {
    case 0x00:
    case '\t':
    case ' ':
    case '(':
    case '|':    goto yy19;
    default:    goto yy29;
    }
yy19:
    YYSETCONDITION(COMMENT);
#line 86 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    goto cond_COMMENT;
#line 145 "lexer_p.h"
yy20:
    state.ctxMarker = state.lexemeEnd + 1;
    ++state.lexemeEnd;
    switch ((yych = *state.lexemeEnd)) {
    case '\t':
    case '\n':
    case ' ':
    case '\'':    goto yy21;
    default:    goto yy37;
    }
yy21:
#line 102 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::Error; }
#line 159 "lexer_p.h"
yy22:
    ++state.lexemeEnd;
    YYSETCONDITION(SINGULAR_SPACE);
#line 87 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::SpaceBegin; }
#line 165 "lexer_p.h"
yy24:
    yych = *++state.lexemeEnd;
    switch (yych) {
    case 0x00:    goto yy14;
    case '\\':    goto yy30;
    default:    goto yy28;
    }
yy25:
    yych = *(state.marker = ++state.lexemeEnd);
    switch (yych) {
    case 0x00:
    case '|':    goto yy21;
    default:    goto yy32;
    }
yy26:
    ++state.lexemeEnd;
    switch ((yych = *state.lexemeEnd)) {
    case 0x00:
    case '\t':
    case ' ':
    case '(':
    case '|':    goto yy27;
    default:    goto yy29;
    }
yy27:
#line 88 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::SpaceBegin; }
#line 193 "lexer_p.h"
yy28:
    ++state.lexemeEnd;
    if (state.inputBufferEnd <= state.lexemeEnd) refill(1);
    yych = *state.lexemeEnd;
yy29:
    switch (yych) {
    case 0x00:
    case '\t':
    case ' ':
    case '(':
    case '|':    goto yy14;
    case '\\':    goto yy30;
    default:    goto yy28;
    }
yy30:
    ++state.lexemeEnd;
    if (state.inputBufferEnd <= state.lexemeEnd) refill(1);
    yych = *state.lexemeEnd;
    switch (yych) {
    case 0x00:    goto yy14;
    case '\\':    goto yy30;
    default:    goto yy28;
    }
yy32:
    ++state.lexemeEnd;
    if (state.inputBufferEnd <= state.lexemeEnd) refill(1);
    yych = *state.lexemeEnd;
    switch (yych) {
    case 0x00:    goto yy34;
    case '|':    goto yy35;
    default:    goto yy32;
    }
yy34:
    state.lexemeEnd = state.marker;
    goto yy21;
yy35:
    ++state.lexemeEnd;
#line 92 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::Atom; }
#line 233 "lexer_p.h"
yy37:
    ++state.lexemeEnd;
    state.lexemeEnd = state.ctxMarker;
#line 90 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::Quotation; }
#line 239 "lexer_p.h"
/* *********************************** */
cond_SINGULAR_SPACE:
    if ((state.inputBufferEnd - state.lexemeEnd) < 2) refill(2);
    yych = *state.lexemeEnd;
    switch (yych) {
    case 0x00:    goto yy41;
    case '\'':    goto yy45;
    case ')':    goto yy47;
    case '\\':    goto yy49;
    default:    goto yy43;
    }
yy41:
    ++state.lexemeEnd;
#line 78 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { --state.lexemeEnd; return Token::Category::Error; }
#line 255 "lexer_p.h"
yy43:
    ++state.lexemeEnd;
#line 99 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::Atom; }
#line 260 "lexer_p.h"
yy45:
    state.ctxMarker = state.lexemeEnd + 1;
    ++state.lexemeEnd;
    switch ((yych = *state.lexemeEnd)) {
    case '\'':    goto yy46;
    default:    goto yy52;
    }
yy46:
#line 102 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::Error; }
#line 271 "lexer_p.h"
yy47:
    ++state.lexemeEnd;
    YYSETCONDITION(REGULAR_SPACE);
#line 98 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::SpaceEnd; }
#line 277 "lexer_p.h"
yy49:
    yych = *++state.lexemeEnd;
    ++state.lexemeEnd;
#line 97 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { ++state.lexemeStart; return Token::Category::Atom; }
#line 283 "lexer_p.h"
yy52:
    ++state.lexemeEnd;
    state.lexemeEnd = state.ctxMarker;
#line 96 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"
    { return Token::Category::Quotation; }
#line 289 "lexer_p.h"
}
#line 103 "/home/eldar/MyProjs/room/src/lexer/src/lexer_p"

}

void unescapeAtomLexeme(Token::Lexeme &str) {
    if (!str.empty() && (str.front() == '|')) {         // multiescaped atom
        if ((str.size() > 1) && (str.back() == '|')) {
            // removing the surrounding boundaries
            str.pop_back();
            str.erase(0, 1);
        }
    } else {
        // removing back-slash espapes
        bool escapeVisited = false;
        auto trash = std::remove_if(str.begin(), str.end(),
                                    [&escapeVisited](Token::Lexeme::value_type c) {
            escapeVisited = (c == '\\' && !escapeVisited);
            return escapeVisited;
        });
        str.erase(trash, str.end());
    }
}

} // namespace details
} // namespace lexer


class Lexer::Private {
public:
    Private(std::streambuf *sb, std::size_t internalBufferSize)
        : _source(sb), _buffer(internalBufferSize)
    {
        re2cState.cond = YYCONDTYPE::REGULAR_SPACE;
        re2cState.inputBufferEnd = _buffer.data() + _buffer.size();
        re2cState.lexemeStart = re2cState.inputBufferEnd;
        re2cState.lexemeEnd = re2cState.inputBufferEnd;
        re2cState.ctxMarker = re2cState.inputBufferEnd;
        re2cState.marker = re2cState.inputBufferEnd;
    }

    void refill(std::size_t);

    /**
     * @brief offset - describes the current offset (in global source) of the current lexeme.
     * @return offset in global source.
     *
     * The informational function needed for Error messages, used by Lexer.
     */
    std::size_t currentOffset() const noexcept;

    /**
     * @brief stash - lengthy lexemes prefix
     *
     * for variable-length lexemes (such as atoms), which size exceed the size of the buffer
     * in such case, after the token-processing will be done, the stash will be contain the
     * preffix of the lexeme.
     */
    lexer::Token::Lexeme stash;
    lexer::detail::Re2cLexerState re2cState;
    bool atEnd = false;


private:
    std::streambuf *_source;
    std::vector<lexer::detail::Re2cLexerState::CharType> _buffer;

    std::size_t _offset = 0;    // informational variable, necessary for Error messages
};

std::size_t Lexer::Private::currentOffset() const noexcept {
    const auto *base = _buffer.data();
    const auto *lexemeStart = re2cState.lexemeStart;
    return _offset  - _offset % _buffer.size() + std::distance(base, lexemeStart);
}

void Lexer::Private::refill(std::size_t n) {
    auto *base = _buffer.data();

    // explicit unsigned type definition (std::size_t) is necessary to prevent compiler warnings
    // when signed-unsigned comparisions in code below
    std::size_t skippedHeadSize = std::distance(base, re2cState.lexemeStart);   // size_t: the 're2cState.lexemeStart' can't precede the 'base'
    auto passedLexemePartSize = std::distance(re2cState.lexemeStart, std::min(re2cState.lexemeEnd,
                                                                              std::min(re2cState.ctxMarker, re2cState.marker)));

    /*
     * buffer: xxxxllllllllee
     *
     * legend:
     *     xxxx     -   skippedHeadSize elements
     *     llllllll -   passedLexemePartSize elements (this region pointed by re2cState.lexemeStart)
     *     ee       -   elements pointed by the either re2cState.lexemeEnd, re2cState.ctxMarker or re2cState.marker
     */
    if (n <= skippedHeadSize) { // we have enough space at beginning of the buffer, let's use it by shifting to the left the rest of the buffer
        n = skippedHeadSize;

        /*
         * xxxxllllllllee
         *     <--- shift to the left, by erasing the xxxx
         *
         * after the shifting:
         * llllllllee????
         *
         * legend:
         *   ????   -   elements which need to be filled
         */
        std::copy(base + n, re2cState.inputBufferEnd, base);
    } else if (n <= (skippedHeadSize + passedLexemePartSize)) { // stash the passed part of the lexeme into the temporary space
        n = skippedHeadSize + passedLexemePartSize;

        /*
         * xxxxllllllllee
         *      \____/
         *         \    copy to the stash
         *          v
         *         stash
         */
        std::copy(re2cState.lexemeStart, re2cState.lexemeStart + passedLexemePartSize, std::back_inserter(stash));

        /*
         * xxxxllllllllee
         *             <-- shift to the left, by erasing the xxxxllllllll
         *
         * after the shifting:
         * ee????????????
         *
         * legend:
         *   ????   -   elements which need to be filled
         */
        std::copy(base + n, re2cState.lexemeEnd, base);
    } else {
        // this can happen if the buffer size is fewer than maximum keyword length in the language
        throw std::runtime_error{"Lexer::Private::refill, can't find the enough space for refilling"};
    }

    // actualize state-pointers
    re2cState.lexemeStart = base;
    re2cState.lexemeEnd -= n;
    re2cState.ctxMarker -= n;
    re2cState.marker -= n;

    auto fillFrom = base + _buffer.size() - n;
    auto filled = _source->sgetn(fillFrom, n);       // fill rest of buffer from source as much as possible
    _offset += filled;

    // if we filled fewer elements then we can (i.e. source ended), then fill rest elements by zero (see nextToken, EOI)
    std::fill_n(fillFrom + filled, n - filled, 0);
}

} // namespace room

#endif // ROOM_LEXER_PRIVATE_H

