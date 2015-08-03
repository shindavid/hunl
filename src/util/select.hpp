#pragma once

struct branchless {
  template<typename T> static T select(bool pred, T a, T b) {
    T arr[2] = {a, b};
    return arr[pred ^ 1];
  }

  /*
  template<typename T1, typename T2,
    typename = decltype( T1( std::declval<T2>() ) )
    >
  inline static decltype(auto) select( bool pred, T1&& a, T2&& b ) {
    using T = typename std::decay<T1>::type;

    T arr[2] = { std::forward<T1>(a), std::forward<T2>(b) } ;
    return arr[pred ^ 1] ;
  }
  */
} ;

/*
 * Returns {0,1,2} if {a<b, a==b, a>b}.
 */
template<typename T> int cmp(T a, T b) {
  return branchless::select(a<b, 0, branchless::select(a==b, 1, 2));
}

