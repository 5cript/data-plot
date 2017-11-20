#pragma once

namespace DataPlot
{
    namespace Expressions
    {
        template <typename T>
        class Value
        {
        public:
            using value_type = T;

            constexpr Value(T value)
                : value_{value}
            {
            }

            constexpr value_type operator() const
            {
                return value_;
            }

        private:
            T value_;
        };
    }
}
