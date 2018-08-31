static const char *err[] = {"Can not access INPUT FILE: ","Can not access OUTPUT FILE: ",nullptr};

enum{
	err_in,err_out
};

static void showerr (std::size_t id,const char *err[],const char *str)
{
	std::cerr << "\nERROR ID " << id  << " : " << err[id] << (str?str:"") << std::endl;
}

static  const char *grappath(const char *path)
{
	const char *i=path,*j=path;
	
	for(;*j;++j) if (*j=='\\' || *j =='/') i=j;
	
	return (i==path)?i:i+1;
}

static void usage(const char *pname,const char *str)
{
	auto gpname=grappath(pname);
	
	std::cout << std::endl << gpname << " is " << str << std::endl << std::endl
						   << gpname << " input-file" << std::endl
						   << gpname << " input-file output-file" << std::endl << std::endl;
}

#define BSIZE (4*1024)