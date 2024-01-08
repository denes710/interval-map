# Interval Map

```interval_map<K, V>``` is a data structure that efficiently associates intervals of keys of type K with values of type
V. It is implemented on top of [```std::map```](https://en.cppreference.com/w/cpp/container/map).

## ```assign``` member function
Each key-value-pair (K, V) in the ```std::map``` means that the value V is associated with the interval from K
(including) to the next key (excluding) in the ```std::map```.

The representation in the ```std::map``` is canonical. Thus, consecutive map entries must not have the same
value, for example ..., (0,'A'), (3,'A'), ... is not allowed. Initially, the whole range of K is associated with a
given initial value, passed to the constructor of the ```interval_map<K, V>``` data structure.

### Complexity
The complexity of ```assign``` member function is log(N).

## Key Type K
* besides being copyable and assignable, is less-than comparable via operator<
* is bounded below, with the lowest value being std::numeric_limits<K>::lowest()
* does not implement any other operations, in particular no equality comparison or arithmetic operators

## Value Type V
* besides being copyable and assignable, is equality-comparable via operator==
* does not implement any other operations
