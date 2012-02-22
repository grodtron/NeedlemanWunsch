#ifndef UTILS_CPP_
#define UTILS_CPP_

template <typename T>
inline T max(T x, T y, T z){
   return (x > y) ? (x > z ? x : z) : (y > z ? y : z);
}

// included directly because of this template function... apparently this
// is what you have to do for templates...
template <typename T>
void swap(T & a, T & b){
   T tmp = a;
   a = b;
   b = tmp;
}

#endif
