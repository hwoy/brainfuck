#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif


#include <utility>
#include <list>

#ifndef _COPT_H__
#define _COPT_H__

#if __cplusplus > 201402L
	#include <string_view>
	typedef std::string_view str_t;
#else
	#include <string>
typedef std::string str_t;
#endif


struct Copt :public std::list<str_t>{

    int argc;
	const char** argv;
    unsigned int start;
	unsigned int index;


    enum ID : unsigned int { END = -2U,
        OTHER = -1U };
		
	Copt()=default;

	template <typename iterator>
    Copt(int argc, const char** argv, iterator bparam, iterator eparam, unsigned int start = 1)
	:std::list<str_t>(bparam,eparam),
	argc(argc),argv(argv),start(start),index(start)
    {
    }
	
	template <typename T,std::size_t N>
    Copt(int argc, const char** argv, const T (&param)[N], unsigned int start = 1)
        : Copt(argc,argv,param, param+N, start)
    {
    }

	template <typename iterator>
    void init(int argc, const char** argv, iterator bparam, iterator eparam, unsigned int start = 1)
    {
        this->argc = argc;
        this->argv = argv;
        assign(bparam, eparam);
        this->index = start;
        this->start = start;
    }

	template <typename T,std::size_t N>	
    void init(int argc, const char** argv, const T (&param)[N], unsigned int start = 1)
    {
		init(argc,argv,param,param+N,start);
    }

    std::pair<unsigned int, str_t> action()
    {

        for (unsigned int j = index; j < reinterpret_cast<unsigned int&>(argc); j++) {

            for (auto i = begin(); i != end(); ++i) {

                if (*i==str_t(argv[j],i->size())) {
                    index = j + 1;
                    return std::make_pair(std::distance(begin(),i), str_t(argv[j] + i->size()));
                }
            }

            index = j + 1;

            return std::make_pair(ID::OTHER, str_t(argv[j]));
        }

        return std::make_pair(ID::END, str_t());
    }
};
#endif
