

struct branchless {

  template<typename T1, typename T2,
    typename = decltype( T1( std::declval<T2>() ) )
    >
  inline static decltype(auto) select( bool pred, T1&& a, T2&& b ) {
    using T = typename std::decay<T1>::type;

    T arr[2] = { std::forward<T1>(a), std::forward<T2>(b) } ;
    return arr[pred ^ 1] ;
  }
} ;
