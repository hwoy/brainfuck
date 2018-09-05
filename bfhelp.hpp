struct path{
	static constexpr const char * const grap(const char * const path)
	{
		return grap(path,path);
	}
	
	private:
	static constexpr const char * const grap(const char * const gpath,const char * const path)
	{
		return *path?  grap( (*path=='\\' || *path =='/') ? path+1 : gpath,path+1) : gpath;
	}
};

static void showerr (std::size_t id,const char *err[],const char *str)
{
	std::cerr << "\nERROR ID " << id  << " : " << err[id] << (str?str:"") << std::endl;
}


static void usage(const char *pname,const char *str)
{
	auto gpname=path::grap(pname);
	
	std::cout << std::endl << gpname << " is " << str << std::endl << std::endl
						  << gpname << " input-file" << std::endl
						  << gpname << " input-file output-file" << std::endl << std::endl;
}

static const char *err[] = {"Can not access INPUT FILE: ","Can not access OUTPUT FILE: ",nullptr};

enum{
	err_fin,err_fout
};


#define BSIZE (4*1024)

#ifndef COL
#define COL (64*2)
#endif

#ifndef LOOPLIMIT
#define LOOPLIMIT 64
#endif

#ifndef BFCELL
#define BFCELL 512
#endif

#ifndef TBFCELL
#define TBFCELL 2
#endif
