#if !BOOST_PP_IS_ITERATING
# error Do not include object_call.hpp directly!
#endif

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#define N BOOST_PP_ITERATION()

template<BOOST_PP_ENUM_PARAMS(N, class A)>
call_proxy<
    Derived
  , boost::tuples::tuple<
        BOOST_PP_ENUM_BINARY_PARAMS(N, A, const* BOOST_PP_INTERCEPT)
    >
> operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a))
{
    typedef boost::tuples::tuple<
        BOOST_PP_ENUM_BINARY_PARAMS(N, A, const* BOOST_PP_INTERCEPT)
    > arguments;

    return call_proxy<Derived, arguments>(
        derived()
      , arguments(BOOST_PP_ENUM_PARAMS(N, &a))
    );
}

#undef N

