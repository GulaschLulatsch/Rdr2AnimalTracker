#pragma once

#include <map>

template <typename T, typename U>
class CreateMap
{
private:
    std::map<T, U> m_map;
public:
    CreateMap(const T& key, const U& val)
    {
        m_map[key] = val;
    }

    CreateMap<T, U>& operator()(const T& key, const U& val)
    {
        m_map[key] = val;
        return *this;
    }

    operator std::map<T, U>()
    {
        return m_map;
    }
};
