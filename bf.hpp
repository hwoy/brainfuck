#ifndef __BF_HPP__

#define __BF_HPP__

#include <iostream>
#include <memory>
#include <exception>
#include <vector>

using cdata_t = char;
using ip_t = std::vector<cdata_t>;

template <class T,class U, class V>
std::basic_ostream<T,U> &operator << (std::basic_ostream<T,U> &out ,const V &ip)
{
	for(const auto &i:ip)
		out << i;
	
	return out;
}

class Bfexception : public std::exception
{
	const char *msg;
	unsigned int id;
	
	public:
	Bfexception(std::size_t eid):msg(exc[eid]),id(eid) {}
	~Bfexception(){}
	
	const char* what() const noexcept
	{
		return msg;
	}
	
	static const char *exc[];
	enum eid
	{
		eid_incptr,
		eid_decptr,
		eid_addptr,
		eid_subptr,
		eid_while,
		eid_endwhile
	};
};

const char *Bfexception::exc[]={
	"Can not increase PTR Cell. Please increase mem Cell.",
	"Can not decrease PTR Cell. Please check your code.",
	"Can not Addition PTR Cell. Please increase mem Cell.",
	"Can not Subtraction PTR Cell. Please check your code.",
	"[ must be end with ]. Please check your code.",
	"] must be begin with [. Please check your code.",
nullptr};

class Cell: protected std::unique_ptr<cdata_t[]>
{
	protected:
	
	cdata_t *ptr;
	std::size_t length;
	
	public:
	
	Cell(std::size_t s=512)
	{
		setcell(s);
	}
	
	void setcell(std::size_t s)
	{
		reset(new cdata_t[s]);
		ptr=get();
		length=s;
		
		for(std::size_t i=0;i<length;++i)
			get()[i] = 0;	
	}
	
	void destroycell(void)
	{
		release();
		ptr=nullptr;
		length=0;
	}
	
	const cdata_t * const getptr() const {return ptr;}
	
	std::size_t getlength() const {return length;}
	
	cdata_t * const operator ++ () 
	{
		if(ptr+1>=get()+length) throw Bfexception(Bfexception::eid_incptr);
		
		return ++ptr;
	}
	
	cdata_t * const operator -- () 
	{
		if(ptr-1<get()) throw Bfexception(Bfexception::eid_decptr);
		
		return --ptr;
	}
	
	cdata_t & operator * () const {return *ptr;}
	
	cdata_t * const operator + (std::size_t n) const 
	{
		if(ptr+n>=get()+length) throw Bfexception(Bfexception::eid_addptr);
		
		return (ptr+n);
	}
	
	cdata_t * const operator - (std::size_t n) const 
	{
		if(ptr-n<get()) throw Bfexception(Bfexception::eid_subptr);
		
		return (ptr-n);
	}
	
};

class Brainfuck
{
	std::ostream out;

	template <class T>
	static std::pair<T,int> fn(int n, T i, T j)
	{
		while(n && i!=j)
		{
			if(*i == '[') ++n;
			else if (*i == ']') --n;
			
			if(n) ++i;
		}
		
		return std::make_pair(i,n);
	}

	template <class T>
	static std::pair<T,int> fp(int n, T i, T j)
	{
		while(n && i!=j)
		{
			if(*i == '[') ++n;
			else if (*i == ']') --n;
			
			if(n) --i;
		}
		
		return std::make_pair(i,n);
	}
	
	public:
	static const cdata_t inst[];
	
	Brainfuck(std::streambuf *rd=std::cout.rdbuf()):out(rd){}

	template <class T>
	int kernel (Cell &cell,T begin, T end,int n=0)
	{
		auto i=begin;
		
		
		while(i != end)
		{
			switch (*i)
			{
				case '>': ++cell;  break;
				case '<': --cell;  break;
				
				case '+': ++(*cell); break;
				case '-': --(*cell); break;

				case '.': out.write(cell.getptr(),1); out.flush(); break;
				case ',': *cell = std::cin.get(); break;
				
				case '[': 
					if (*cell == 0)
						std::tie(i,n) =fn(1,++i,end);

					break;
					
				case ']': std::tie(i,n) =fp(-1,--i,begin);
					continue;
			}
			
			if(i!=end) ++i;
		}
		
		return n;
	}	
	

	template <class T>
	int operator() (Cell &cell,const T &ip)
	{
		int n = kernel(cell,ip.begin(),ip.end());
		
		if(n>0) throw Bfexception(Bfexception::eid_while);
		else if(n<0) throw Bfexception(Bfexception::eid_endwhile);
		
		return n;
	}
};
	
	const cdata_t Brainfuck::inst[] = "><+-.,[]";

#endif