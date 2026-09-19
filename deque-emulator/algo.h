#pragma once

#include <cstddef>
#include <deque>

template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1,const std::deque<T>& half2,
    const Comp& comparator){
    std::deque<T> result;

    auto it1 = half1.begin();
    auto it2 = half2.begin();

    while (it1 != half1.end() && it2 != half2.end()){
        if (comparator(*it2, *it1)){
        result.push_back(*it2);
        ++it2;
        }
        else{
            result.push_back(*it1);
            ++it1;
        }
    }

    while (it1 != half1.end()){
        result.push_back(*it1);
        ++it1;
    }

    while (it2 != half2.end()){
        result.push_back(*it2);
        ++it2;
    }

    return result;
}


template<class T, class Comp> std::deque<T> MergeSort(const std::deque<T>& src,
    const Comp& comparator){

    if (src.size() <= 1){
        return src;
    }

    const std::size_t middle = src.size() / 2;

    std::deque<T> half1(src.begin(),src.begin() + static_cast<std::ptrdiff_t>(middle));

    std::deque<T> half2(src.begin() + static_cast<std::ptrdiff_t>(middle),src.end());

    half1 = MergeSort(half1, comparator);
    half2 = MergeSort(half2, comparator);

    return Merge(half1, half2, comparator);
}

template<class Container, class Comp>Container Merge(const Container& half1,
    const Container& half2, const Comp& comparator)
{
    Container result;

    auto it1 = half1.begin();
    auto it2 = half2.begin();

    while (it1 != half1.end() && it2 != half2.end())
    {
        if (comparator(*it2, *it1)){
            result.push_back(*it2);
            ++it2;
        }
        else{
            result.push_back(*it1);
            ++it1;
        }
    }

    while (it1 != half1.end()){
        result.push_back(*it1);
        ++it1;
    }

    while (it2 != half2.end()){
        result.push_back(*it2);
        ++it2;
    }

    return result;
}


template<class Container, class Comp>
Container MergeSort(const Container& src, const Comp& comparator)
{
    if (src.size() <= 1){
        return src;
    }

    const std::size_t middle = src.size() / 2;

    Container half1(src.begin(), src.begin() +
            static_cast<std::ptrdiff_t>(middle)
    );

    Container half2(src.begin() + static_cast<std::ptrdiff_t>(middle),src.end());

    half1 = MergeSort(half1, comparator);
    half2 = MergeSort(half2, comparator);

    return Merge(half1, half2, comparator);
}
