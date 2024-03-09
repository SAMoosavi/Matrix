#ifndef MATRIX_CONCEPT_H
#define MATRIX_CONCEPT_H

#include <iostream>
#include <vector>
#include <ranges>

using namespace std;
template<typename T>
concept Multiplicationable = requires(T t) {
    { t * t } -> same_as<T>;
};
template<typename T>
concept AssignMultiplicationable = requires(T t) {
    { t *= t } -> same_as<T &>;
};

template<typename T>
concept Sumable = requires(T t) {
    { t + t }-> same_as<T>;
};
template<typename T>
concept AssignSumable = requires(T t) {
    { t += t }-> same_as<T &>;
};


template<typename Container, typename Element = typename Container::value_type>
concept Containerable = requires(Container c, Element e) {
    requires ranges::range<Container>;
    { c.size() }-> same_as<ranges::range_size_t<Container>>;
    typename Element;
};

template<typename Element>
concept Elementable = requires(Element){
	requires Multiplicationable<Element>;
	requires Sumable<Element>;
};


#endif