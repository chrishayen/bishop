/**
 * @file asio_impl.cpp
 * @brief Boost.Asio separate compilation implementation.
 *
 * This file contains the compiled implementation of Boost.Asio.
 * By using BOOST_ASIO_SEPARATE_COMPILATION, user programs only need
 * to parse Asio declarations (not implementations), significantly
 * reducing compile times.
 *
 * See: https://www.boost.org/doc/libs/1_85_0/doc/html/boost_asio/using.html
 */

#define BOOST_ASIO_SEPARATE_COMPILATION
#include <boost/asio/impl/src.hpp>
