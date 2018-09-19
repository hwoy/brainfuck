#ifndef __BF_HPP__

#define __BF_HPP__

#include <iostream>
#include <memory>
#include <exception>
#include <vector>
#include <string>

#ifndef CELL_T

#define CELL_T char

#endif

using cell_t = CELL_T;

using inst_t = char;
using ip_t = std::vector<inst_t>;


class Bfexception final: public std::exception
{
	const std::string msg;
	const unsigned int id;
	
	public:
	Bfexception(const std::size_t eid):msg(exc[eid]),id(eid) {}
	Bfexception(std::string &&msg):msg(std::move(msg)),id(-1U) {}
	
	
	~Bfexception() override {}
	
	const char* what() const noexcept override
	{
		return msg.c_str();
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
	"Can not increase PTR Tape. Please increase mem Tape.",
	"Can not decrease PTR Tape. Please check your code.",
	"Can not Addition PTR Tape. Please increase mem Tape.",
	"Can not Subtraction PTR Tape. Please check your code.",
	"[ must be end with ]. Please check your code.",
	"] must be begin with [. Please check your code.",
nullptr};

class Tape: protected std::unique_ptr<cell_t[]>
{
	protected:
	
	cell_t *ptr;
	std::size_t length;
	
	public:
	
	Tape(std::size_t s=512)
	{
		settape(s);
	}
	
	void settape(std::size_t s)
	{
		reset(new cell_t[s]);
		ptr=get();
		length=s;
		
		for(std::size_t i=0;i<length;++i)
			get()[i] = 0;	
	}
	
	void destroytape(void)
	{
		release();
		ptr=nullptr;
		length=0;
	}
	
	const cell_t * const getbaseptr() const
	{
		return get();
	}
	
	const cell_t * const getptr() const {return ptr;}
	
	cell_t * const getptr_mutable() const {return ptr;}
	
	std::size_t getlength() const {return length;}
	
	cell_t * const operator ++ () 
	{
		if(ptr+1>=get()+length) throw Bfexception(Bfexception::eid_incptr);
		
		return ++ptr;
	}
	
	cell_t * const operator -- () 
	{
		if(ptr-1<get()) throw Bfexception(Bfexception::eid_decptr);
		
		return --ptr;
	}
	
	cell_t & operator * () const {return *ptr;}
	
	cell_t * const operator + (std::size_t n) const 
	{
		if(ptr+n>=get()+length) throw Bfexception(Bfexception::eid_addptr);
		
		return (ptr+n);
	}
	
	cell_t * const operator - (std::size_t n) const 
	{
		if(ptr-n<get()) throw Bfexception(Bfexception::eid_subptr);
		
		return (ptr-n);
	}	
};

class Brainfuck
{
	protected:
	
	std::ostream out;

	template <class T>
	static std::pair<T,int> openbracket(T i, T j,int n=1)
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
	static std::pair<T,int> closebracket(T i, T j,int n=-1)
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
	static const inst_t inst[];
	
	Brainfuck(std::streambuf *rd=std::cout.rdbuf()):out(rd){}

	template <class T>
	int eval (Tape &tape,T begin, T end,int n=0)
	{
		auto ip=begin;
		
		
		while(ip != end)
		{
			switch (*ip)
			{
				case '>': ++tape;  													break;
				case '<': --tape;  													break;
				
				case '+': ++(*tape);													break;
				case '-': --(*tape);													break;

				case '.': out << static_cast<char>(*tape.getptr()); out.flush(); break;
				case ',': *tape.getptr_mutable() = std::cin.get();	 break;
				
				case '[': if (!*tape)	std::tie(ip,n) = openbracket(++ip,end);		break;	
				case ']': if (*tape)		std::tie(ip,n) = closebracket(--ip,begin);	break;
				
				default:															break;
			}
			
			if(ip!=end) ++ip;
		}
		
		return n;
	}	
	
};
	
	const inst_t Brainfuck::inst[] = "><+-.,[]";

#endif