#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>
#include <memory>

#include "bf.hpp"

using factor_t = std::pair<unsigned int,unsigned int>;


std::vector<factor_t> factor(unsigned int n)
{
	std::vector<factor_t> vec;
	
	for(unsigned int i=1;i<= (n/2)+1 ;++i)
	{
		if(!(n%i)) vec.push_back({i,n/i});
	}
	return vec;
}

factor_t minfactor(const std::vector<factor_t> &vec)
{
	return *std::min_element(vec.begin(),vec.end(),
	[](const factor_t &a,const factor_t &b)
	{
		return (a.first + a.second) < (b.first + b.second);
	});
}

bool isprime(unsigned int n)
{
	for(unsigned int i=2;i*i<=n;++i)
	{
		if(!(n%i)) return false;
	}
	
	return true;
}

unsigned int findfactor(unsigned int n)
{
	while(isprime(n)) n--;
	
	return n;
}

std::string a2bfA(unsigned int n, Cell &cell)
{
	unsigned int a,b;
	unsigned int m= n>*(cell+1) ? n-*(cell+1) : *(cell+1)-n;
	std::string str("");
	std::tie(a,b) = minfactor(factor(findfactor(m)));
	
	(a>*cell)? str.append(a-(*cell),'+') : str.append((*cell)-a,'-') ;
	str.append(" [> ");
	
	*cell=0;
	++cell;
	
	(n>*cell)? str.append(b,'+') : str.append(b,'-') ;
	str.append(" <-] > ");
	
	
	if( (n>*cell) && ((*cell+a*b)<n) ) str.append(n-(*cell+a*b),'+')  ;
	else if((*cell>=n) && ((n+a*b)<*cell) ) str.append(*cell-(n+a*b),'-')  ;
	
	str.append(" . < ");

	*cell=n;
	--cell;
	
	return str;
}

std::string a2bfB(unsigned int n, Cell &cell)
{
	std::string str(" > ");
	++cell;
	(n>*cell) ? str.append(n-*cell,'+') : str.append(*cell-n,'-');
	
	str.append(" . < ");
	
	*cell=n;
	--cell;
	
	return str;
}

std::string i2bf(unsigned int n, Cell &cell)
{
	if( ((n>*(cell+1))&&((n-*(cell+1))<=11)) || ((n<=*(cell+1))&&((*(cell+1)-n)<=11))) return a2bfB(n,cell);
	
	return a2bfA(n,cell);
}

template <class T>
std::string it2bf(T i,T j, Cell &cell)
{
	std::string str("");
	
	for(;i != j; ++i)
	{
		str+=i2bf(*i,cell);
	}
	
	return str;
}

#define BSIZE (16)

int main(int argc , const char **argv)
{
if(argc >1)
{

std::ifstream fin(argv[1],std::ios::binary);

if(fin)
{
	Cell cell;
	std::unique_ptr<char[]> buff(new char[BSIZE+1]);
	std::size_t count;
	
	do
	{
		fin.read(buff.get(),BSIZE);
		std::cout << it2bf(buff.get(),buff.get()+(count=fin.gcount()),cell);
	}while(count>=BSIZE);

	
}
else
{
	std::cerr << "can not open file: " << argv[1] << "\n";
}

}
return 0;
}