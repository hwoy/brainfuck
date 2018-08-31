#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>
#include <memory>

#include "bf.hpp"
#include "bfhelp.hpp"

using num_t = cdata_t;

using factor_t = std::pair<num_t,num_t>;


static std::vector<factor_t> factor(num_t n)
{
	std::vector<factor_t> vec;
	
	for(num_t i=1;i<= (n/2)+1 ;++i)
	{
		if(!(n%i)) vec.push_back({i,n/i});
	}
	return vec;
}

static factor_t minfactor(const std::vector<factor_t> &vec)
{
	return *std::min_element(vec.begin(),vec.end(),
	[](const factor_t &a,const factor_t &b)
	{
		return (a.first + a.second) < (b.first + b.second);
	});
}

static bool isprime(num_t n)
{
	for(num_t i=2;i*i<=n;++i)
	{
		if(!(n%i)) return false;
	}
	
	return true;
}

static num_t findfactor(num_t n)
{
	while(isprime(n)) n--;
	
	return n;
}

static ip_t a2bfA(num_t n, Cell &cell)
{
	num_t a,b;
	num_t m= n>*(cell+1) ? n-*(cell+1) : *(cell+1)-n;
	ip_t str;
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

static ip_t a2bfB(num_t n, Cell &cell)
{
	ip_t str(" > ");
	++cell;
	(n>*cell) ? str.append(n-*cell,'+') : str.append(*cell-n,'-');
	
	str.append(" . < ");
	
	*cell=n;
	--cell;
	
	return str;
}

static ip_t i2bf(num_t n, Cell &cell)
{
	if( ((n>*(cell+1))&&((n-*(cell+1))<=11)) || ((n<=*(cell+1))&&((*(cell+1)-n)<=11))) return a2bfB(n,cell);
	
	return a2bfA(n,cell);
}

template <class T>
static ip_t it2bf(T i,T j, Cell &cell)
{
	ip_t str("");
	
	for(;i != j; ++i)
	{
		str+=i2bf(*i,cell);
	}
	
	return str;
}


int main(int argc , const char *argv[])
{

if(argc<=1)
{
	usage(argv[0],"a Text to Brainf**k Encoder.");
	return 0;
}

std::ofstream fout;
std::ifstream fin;


if(argc > 1)
{
	fin.open(argv[1],std::ios::in | std::ios::binary);
	if(!fin) 
	{
		showerr(err_in,err,argv[1]);
		return 1;
	}
}

if(argc > 2)
{
	fout.open(argv[2],std::ios::out | std::ios::binary);
	if(!fout) 
	{
		showerr(err_out,err,argv[2]);
		return 1;
	}
}

Cell cell;
std::ostream out(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );

ip_t str;
std::unique_ptr<cdata_t[]> buff(new cdata_t[BSIZE+1]);
std::size_t count=BSIZE;


	do
	{
		fin.read(buff.get(),BSIZE);
		out << it2bf(buff.get(),buff.get()+(count=fin.gcount()),cell);
	}while(count>=BSIZE);



return 0;
}