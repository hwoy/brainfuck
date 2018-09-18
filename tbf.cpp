#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

#include "bf.hpp"
#include "bfhelp.hpp"


using num_t = int;

using factor_t = std::pair<num_t,num_t>;

using listfactor_t = std::vector<factor_t>;



static listfactor_t factor(num_t n)
{
	listfactor_t vec;
	
	for(num_t i=1;i<= (n/2)+1 ;++i)
	{
		if(!(n%i)) vec.push_back({i,n/i});
	}
	return vec;
}

static factor_t minfactor(const listfactor_t &vec)
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
		
	while(isprime(n) && (n>3)) --n;
	
	return n;
}

static void appendip(ip_t &ip,const std::string &str)
{
	ip.insert(ip.end(),str.begin(),str.end());
}

static ip_t a2bfA(num_t n, Tape &tape)
{
	ip_t ip;
	
	num_t a,b;
	num_t m= n>*(tape) ? n-*(tape) : *(tape)-n;
	std::tie(a,b) = minfactor(factor(findfactor(m)));
	
	
	appendip(ip,">");++tape;
	
	(a>*tape)? ip.insert(ip.end(),a-(*tape),'+') : ip.insert(ip.end(),(*tape)-a,'-') ;
	
	*tape=0;
	
	appendip(ip,"[<");--tape;
	
	(n>*tape)? ip.insert(ip.end(),b,'+') : ip.insert(ip.end(),b,'-') ;
	
	appendip(ip,">-]<");
	
	
	
	if( (n>*tape) && ((*tape+a*b)<n) ) ip.insert(ip.end(),n-(*tape+a*b),'+')  ;
	else if((*tape>=n) && ((n+a*b)<*tape) ) ip.insert(ip.end(),*tape-(n+a*b),'-')  ;
	
	*tape=n;
	appendip(ip,".");
	
	return ip;
}

static ip_t a2bfB(num_t n, Tape &tape)
{
	ip_t ip;
	
	(n>*tape) ? ip.insert(ip.end(),n-*tape,'+') : ip.insert(ip.end(),*tape-n,'-');
	
	*tape=n;

	appendip(ip,".");	
	
	
	return ip;
}

static ip_t movetapeptr(Tape &tape,std::size_t index)
{
	auto diff = (tape.getbaseptr()+index)-tape.getptr();
	ip_t ip;
	
	if(diff>0)	for(;diff;--diff,++tape) ip.push_back('>');
	
	else		for(;diff;++diff,--tape) ip.push_back('<');
	
	return ip;
}

static ip_t movea2bf(num_t n,Tape &tape,std::size_t index)
{
	ip_t ip = movetapeptr(tape,index);
	
	auto ip2=(( ((n>*tape)&&((n-*tape)<=11)) || ((n<=*tape)&&((*tape-n)<=11)) ) ? a2bfB : a2bfA) (n,tape);
	
	ip.insert(ip.end(),ip2.begin(),ip2.end());
	
	return ip;
}

static ip_t a2bf(num_t n, Tape &tape)
{
	
	switch (n)
	{
		case 32:	return movea2bf(n,tape,2);	break;
		
		case 10:	return movea2bf(n,tape,4);	break;
		
		case 13:	return movea2bf(n,tape,6);	break;
		
		default: 	return movea2bf(n,tape,0);
	}
	
}

template <class T>
static const ip_t it2bf(T i,T j, Tape &tape)
{
	ip_t ip;
	
	for(;i != j; ++i)
	{
		auto ip2 = a2bf(*i,tape);
		ip.insert(ip.end(),ip2.begin(),ip2.end());
	}
	
	return ip;
}

static unsigned int printip(std::ostream &out,const ip_t &ip,unsigned int ccol,unsigned int col)
{
	for(const auto &i:ip)
	{
		out << i;
		
		ccol=(ccol+1)%col;
		
		if(!ccol) out << std::endl;
	}
	
	return ccol;
}

int main(int argc , const char *argv[])
{

if(argc<=1)
{
	usage(argv[0],"a Text to Brainf**k Encoder.");
	return 0;
}


std::ifstream fin;


if(argc > 1)
{
	fin.open(argv[1],std::ios::in | std::ios::binary);
	if(!fin) 
	{
		showerr(err_fin,err,argv[1]);
		return 1;
	}
}

std::ofstream fout;

if(argc > 2)
{
	fout.open(argv[2]);
	if(!fout) 
	{
		showerr(err_fout,err,argv[2]);
		return 1;
	}
}


std::ostream out(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );

Tape tape(TBFTAPE);
std::unique_ptr<inst_t[]> buff(new inst_t[BSIZE+1]);

unsigned int ccol = 0;

try{
	
	do
	{
		fin.read(buff.get(),BSIZE);
		ccol=printip(out,it2bf(buff.get(),buff.get()+fin.gcount(),tape),ccol,COL);
	}while(fin.gcount()>=BSIZE);
	
}catch(const std::exception &e)
{
	std::cerr	<< std::endl	<< "Exception:"	<< std::endl
				<< e.what()	<< std::endl;
}



return 0;
}