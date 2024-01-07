#pragma once

#include <map>
#include <string>
#include <stdexcept>

template<typename K, typename V>
class interval_map
{
    public:
        class CCanonicalException : public std::runtime_error
        {
            public:
                CCanonicalException(const std::string& p_exp)
                    : runtime_error(p_exp)
                {}
        };

        // constructor associates whole range of K with p_val
        interval_map(const V& p_val)
            : m_valBegin(p_val)
        {}

        // Assign value p_val to interval [p_keyBegin, p_keyEnd).
        // Overwrite previous values in this interval.
        // If !( p_keyBegin < p_keyEnd ), this designates an empty interval,
        // and assign must do nothing.
        void assign(const K& p_keyBegin, const K& p_keyEnd, const V& p_val)
        {
            if (!(p_keyBegin < p_keyEnd))
                return;

            if (m_map.empty())
            {
                // initialize empty interval map
                if (p_val == m_valBegin)
                    throw CCanonicalException("Value is equal with the default begin value during map initializtion!");
                auto it = m_map.insert(m_map.begin(), {p_keyBegin, p_val});
                m_map.insert(it, {p_keyEnd, m_valBegin});
                return;
            }

            // begin and end of the overwriten interval which will be erased
            using TMapIt = decltype(m_map.begin());
            TMapIt itBegin;
            TMapIt itEnd;

            // first element that is not less than p_keyBegin, otherwise end iterator
            auto lowerBoundBeginIt = m_map.lower_bound(p_keyBegin);
            // first element that is not less than p_keyEnd, otherwise end iterator
            auto lowerBoundEndIt = m_map.lower_bound(p_keyEnd);

            // dealing with the end of the inerval
            if (lowerBoundEndIt != m_map.end())
            {
                if (!(lowerBoundEndIt->first < p_keyEnd) && !(p_keyEnd < lowerBoundEndIt->first))
                {
                    // p_keyEnd is already in the map
                    if (p_val == lowerBoundEndIt->second)
                        itEnd = ++lowerBoundEndIt;
                    else
                        itEnd = lowerBoundEndIt;
                }
                else
                {
                    // we should insert the p_keyEnd
                    if (lowerBoundEndIt == m_map.begin())
                    {
                        // p_keyBegin will be the smallest key
                        itEnd = m_map.insert(lowerBoundEndIt, {p_keyEnd, m_valBegin});
                    }
                    else
                    {
                        auto it = std::prev(lowerBoundEndIt);
                        if (p_val == it->second)
                            itEnd = lowerBoundEndIt;
                        else
                            itEnd = m_map.insert(it, {p_keyEnd, it->second});
                    }
                }
            }
            else
            {
                // p_keyEnd is the highest
                itEnd = m_map.insert(lowerBoundEndIt, {p_keyEnd, m_valBegin});
            }

            // dealing with the begin of the inerval
            if (lowerBoundBeginIt != m_map.end())
            {
                if (lowerBoundBeginIt == m_map.begin())
                {
                    // p_keyBegin will be the smallest key
                    itBegin = m_map.insert_or_assign(lowerBoundBeginIt, p_keyBegin, p_val);
                }
                else
                {
                    auto it = std::prev(lowerBoundBeginIt);
                    if (p_val == it->second)
                        // keep the node with the same key
                        itBegin = it;
                    else
                        // insert node with p_keyBegin
                        itBegin = m_map.insert_or_assign(it, p_keyBegin, p_val);
                }
            }
            else
            {
                // keyStart is the highest key
                m_map.insert(lowerBoundBeginIt, {p_keyBegin, p_val});
                // no erase existing key
                itBegin = m_map.end();
            }

            // erase nodes in the inerval
            if (itBegin != m_map.end())
            {
                if (auto nextIt = std::next(itBegin); nextIt != m_map.end())
                    m_map.erase(nextIt, itEnd);
            }
        }

        // look-up of the value associated with key
        const V& operator[](const K& p_key) const
        {
            auto it = m_map.upper_bound(p_key);
            if (it == m_map.begin())
                return m_valBegin;
            return (--it)->second;
        }

    private:
        V m_valBegin;
        std::map<K,V> m_map;
};
