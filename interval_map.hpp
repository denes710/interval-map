#pragma once

#include <map>

template<typename K, typename V>
class interval_map {
    // FIXME it is only for test
    public:
	V m_valBegin;
	std::map<K,V> m_map;
public:
    // TODO own exception type

    // constructor associates whole range of K with val
    interval_map(V const& val)
        : m_valBegin(val)
    {}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        if (!(keyBegin < keyEnd))
            return;

        if (m_map.empty())
        {
            if (val == m_valBegin)
                throw;
            auto it = m_map.insert(m_map.begin(), {keyBegin, val});
            m_map.insert(it, {keyEnd, m_valBegin});
            return;
        }

        // FIXME it would be better with decltype
        // begin and end of the overwriten interval which will be erased
        typename std::map<K, V>::iterator itBegin;
        typename std::map<K, V>::iterator itEnd;

        // first element that is not less than keyBegin, otherwise end iterator
        auto lowerBoundBeginIt = m_map.lower_bound(keyBegin);
        // first element that is not less than keyEnd, otherwise end iterator
        auto lowerBoundEndIt = m_map.lower_bound(keyEnd);

        // dealing with the end of the inerval
        if (lowerBoundEndIt != m_map.end())
        {
            if (!(lowerBoundEndIt->first < keyEnd) && !(keyEnd < lowerBoundEndIt->first))
            {
                // keyEnd is already in the map
                if (val == lowerBoundEndIt->second)
                    itEnd = ++lowerBoundEndIt;
                else
                    itEnd = lowerBoundEndIt;
            }
            else
            {
                // we should insert the keyEnd
                if (lowerBoundEndIt == m_map.begin())
                {
                    // keyBegin will be the smallest key
                    itEnd = m_map.insert(lowerBoundEndIt, {keyEnd, m_valBegin});
                }
                else
                {
                    auto it = std::prev(lowerBoundEndIt);
                    if (val == it->second)
                        itEnd = lowerBoundEndIt;
                    else
                        itEnd = m_map.insert(it, {keyEnd, it->second});
                }
            }
        }
        else
        {
            // keyEnd is the highest
            itEnd = m_map.insert(lowerBoundEndIt, {keyEnd, m_valBegin});
        }

        // dealing with the begin of the inerval
        if (lowerBoundBeginIt != m_map.end())
        {
            if (lowerBoundBeginIt == m_map.begin())
            {
                // keyBegin will be the smallest key
                itBegin = m_map.insert_or_assign(lowerBoundBeginIt, keyBegin, val);
            }
            else
            {
                auto it = std::prev(lowerBoundBeginIt);
                if (val == it->second)
                    // keep the node with the same key
                    itBegin = it;
                else
                    // insert node with keyBegin
                    itBegin = m_map.insert_or_assign(it, keyBegin, val);
            }
        }
        else
        {
            // keyStart is the highest key
            m_map.insert(lowerBoundBeginIt, {keyBegin, val});
            // no erase existing key
            itBegin = m_map.end();
        }

        // overwritten keys in the inerval
        if (itBegin != m_map.end())
        {
            if (auto nextIt = std::next(itBegin); nextIt != m_map.end())
                m_map.erase(nextIt, itEnd);
        }
    }

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};
