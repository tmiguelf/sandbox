//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\author Tiago Freire
///
///	\copyright
///		Copyright (c) 2020 Tiago Miguel Oliveira Freire
///		
///		Permission is hereby granted, free of charge, to any person obtaining a copy
///		of this software and associated documentation files (the "Software"), to deal
///		in the Software without restriction, including without limitation the rights
///		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
///		copies of the Software, and to permit persons to whom the Software is
///		furnished to do so, subject to the following conditions:
///		
///		The above copyright notice and this permission notice shall be included in all
///		copies or substantial portions of the Software.
///		
///		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
///		SOFTWARE.
//======== ======== ======== ======== ======== ======== ======== ========


#include <optional>

#include <benchmark/benchmark.h>
#include "data.h"

bool alwaysFalse(uintptr_t&) noexcept
{
	return false;
}

bool alwaysTrue(uintptr_t&) noexcept
{
	return true;
}

bool dataAccess(uintptr_t& p_index) noexcept
{
	if(++p_index >= data_size) p_index = 0;
	return data[p_index];
}

using testfun_t = bool (*) (uintptr_t&) noexcept;


std::optional<bool> returnFunc(bool p_val) noexcept
{
	volatile bool test1 = p_val;
	volatile bool test2 = p_val;

	if(test1) return {};
	return test2;
}

void RunReturnTest(benchmark::State& state, testfun_t fun) noexcept
{
	uintptr_t i = 0;

	[[maybe_unused]] volatile bool dump_val = false;

	for (auto _ : state)
	{
		const std::optional<bool>& ret = returnFunc(fun(i));
		if(ret.has_value())
		{
			dump_val = ret.value();
		}
		else
		{
			dump_val = false;
		}
	}
}


void no_except_random(benchmark::State& state) noexcept
{
	RunReturnTest(state, dataAccess);
}

void no_except_always_good(benchmark::State& state) noexcept
{
	RunReturnTest(state, alwaysFalse);
}

void no_except_always_bad(benchmark::State& state) noexcept
{
	RunReturnTest(state, alwaysTrue);
}

BENCHMARK(no_except_random);
BENCHMARK(no_except_always_good);
BENCHMARK(no_except_always_bad);

#if defined (__cpp_exceptions)

bool exceptFunc(bool p_val)
{
	volatile bool test1 = p_val;
	volatile bool test2 = p_val;

	if(test1) throw false;
	return test2;
}


void RunExceptTest(benchmark::State& state, testfun_t fun) noexcept
{
	uintptr_t i = 0;

	[[maybe_unused]] volatile bool dump_val = false;

	for (auto _ : state)
	{
		try
		{
			dump_val = exceptFunc(fun(i));
		}
		catch(...)
		{
			dump_val = false;
		}
	}
}


void except_random(benchmark::State& state) noexcept
{
	RunExceptTest(state, dataAccess);
}

void except_always_good(benchmark::State& state) noexcept
{
	RunExceptTest(state, alwaysFalse);
}

void except_always_bad(benchmark::State& state) noexcept
{
	RunExceptTest(state, alwaysTrue);
}

BENCHMARK(except_random);
BENCHMARK(except_always_good);
BENCHMARK(except_always_bad);

#endif

