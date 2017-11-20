#pragma once

namespace DataPlot
{
    namespace Expressions
    {
        #define BINARY_EXPRESSION(NAME, OPERATOR) \
        template <typename Lhs, typename Rhs> \
        class NAME \
        { \
        public: \
            using value_type = decltype(typename Lhs::value_type + typename Rhs::value_type); \
            \
            value_type operator() const \
            { \
                return Lhs() OPERATOR Rhs(); \
            } \
        }
    }
}

