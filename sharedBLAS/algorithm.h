#pragma once

/* Author: jrmwng @ 2016 */

#include <utility>

template <typename Tfunc, typename Tint, Tint... tInt>
void for_each(std::integer_sequence<Tint, tInt...>, Tfunc && tFunc)
{
	using for_each_type = int [];
	for_each_type{
		(std::forward<Tfunc>(tFunc)(std::integral_constant<Tint, tInt>()), 0)...
	};
}
