#pragma once

#include <vector>
#include <functional>

namespace DataPlot
{
    template <typename T>
    class DataSet
    {
    public:
        DataSet() = default;
        DataSet(DataSet const&) = default;
        DataSet(DataSet&&) = default;
        DataSet& operator=(DataSet const&) = default;
        DataSet& operator=(DataSet&&) = default;

        /**
         *  Removes all points from the data set.
         */
        void clear()
        {
            data_.clear();
        }

        /**
         *  Inserts a series of values into the set, with x incrementing automatically.
         */
        template <typename IncrementT>
        void insertIncremented(std::vector <T> const& y, IncrementT xStart = {}, IncrementT xIncrement = IncrementT{1})
        {
            IncrementT x = xStart;
            for (auto const& i : y)
            {
                insert(static_cast <T> (x), i);
                x += xIncrement;
            }
        }

        /**
         *  Returns whether or not the data is empty.
         */
        bool empty() const
        {
            return data_.empty();
        }

        /**
         *  Inserts a series of values into the set, with y being generated on the fly from each x value.
         */
        void insert(
            std::vector <T> const& x,
            std::function <T(T)> const& yGenerator
        )
        {
            for (auto const& i : x)
                insert(x, yGenerator(x));
        }

        /**
         *  Adds a point to the dataset.
         *
         *  @param point A point in 2D space (first = x, second = y).
         */
        void insert(std::pair <T, T> const& point)
        {
            insert(point.first, point.second);
        }

        /**
         *  Adds a point to the dataset.
         *
         *  @param x The x coordinate.
         *  @param y The y value.
         */
        void insert(T x, T y)
        {
            if (data_.empty())
            {
                min_ = y;
                max_ = y;
            }
            else
            {
                if (y > max_)
                    max_ = y;
                if (y < min_)
                    min_ = y;
            }
            data_.emplace_back(x, y);
        }

        typename std::vector <std::pair <T, T> >::iterator begin()
        {
            return std::begin(data_);
        }
        typename std::vector <std::pair <T, T> >::iterator end()
        {
            return std::end(data_);
        }
        typename std::vector <std::pair <T, T> >::const_iterator begin() const
        {
            return std::begin(data_);
        }
        typename std::vector <std::pair <T, T> >::const_iterator end() const
        {
            return std::end(data_);
        }

        constexpr T max() const
        {
            return max_;
        }
        constexpr T min() const
        {
            return min_;
        }

        std::size_t size() const
        {
            return data_.size();
        }

    private:
        std::vector <std::pair <T, T> > data_;
        T max_;
        T min_;
    };
}
