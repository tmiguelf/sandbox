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


#include <cstdint>
#include <random>
#include <fstream>

constexpr uintptr_t ammountX = 1000;
constexpr uintptr_t ammountY = 10000;

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> distrib(0, 255);

	std::ofstream genfile;

	genfile.open("data.h", std::ios_base::out);

	genfile
		<< "#pragma once \n\n"
		<< "const bool data[] = {\n\t";

	for(uintptr_t i = 0; i < ammountY; ++i)
	{
		for(uintptr_t j = 0; j < ammountX; ++j)
		{
			genfile << ' ' << ((distrib(gen) & 1) ? '1' : '0' /*true : false*/) << ',';
		}
		genfile << '\n' << '\t';
	}

	genfile << "};\n\n";

	genfile << "constexpr uintptr_t data_size = sizeof(data);\n";

	return 0;
}
