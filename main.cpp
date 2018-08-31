#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>



#include "card.hpp"
#include "player.hpp"
#include "52.hpp"
#include "52io.hpp"

#include "52config.hpp"
#include "52type.hpp"

#include "Copt.hpp"

#include "computer.hpp"
#include "human.hpp"

using namespace std::chrono;

template <typename It>
static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck,It it, It itbase);

template <std::size_t M>
static void showhelp(const char* (&opt)[M],const char* (&opt_des)[M]);

template <typename T,typename F>
static T findnext(T it,T begin, T end ,const F &f);

template <std::size_t M,std::size_t N>
static deck_t constructdeck(const rank_t (&rank)[M],const suit_t (&suit)[N]);


static void showwinner(const player_t &player,unsigned int money);

static void showwinnerofthematch(const player_t &player);


static const rank_t rank[]={ {0,"A",1} , {1,"2",2} , {2,"3",3} ,\
								   {3,"4",4} , {4,"5",5} , {5,"6",6} ,\
								   {6,"7",7} , {7,"8",8} , {8,"9",9} ,\
								   {9,"10",10} , {10,"J",10} , {11,"Q",10} , {12,"K",10} };
					  
static const suit_t suit[]={{0,"Spade",0} , {1,"Heart",1} , {2,"Diamon",2}, {3,"Clover",3}};


static const char* option[] = { "-b:", "-m:", "-h:", "-c:", "-c2:", "-H" };

static const char* option_des[] = { "-b:money <Bid money>", "-m:money <Money>", "-h:name <Human>", "-c:name <Computer>","-c2:name <Computer2>", "Help" };

enum optid : unsigned int 
{ 	opt_b,
    opt_m,
    opt_h,
    opt_c,
	opt_c2,
    opt_H };
	
	
enum errid : unsigned int 
{ 	err_nan,
    err_invalidopt,
    err_bid,
    err_money,
    err_player,
	err_bidmoney,
	err_up2player,
	err_empty,
	err_biddrawmoney};
	
static const char* err_des[] = { "Not a positive number", "Invalid option", "Less than BID", "Less than MONEY", "Less than MAXPLAYER","bid more than money","up to 2 players","Empty name","More Bid or Less Money" };

static int showerr(const char** err_des,unsigned int id,const std::string &str);

static bool isdigital(const string_t &str)
{	
	return !str.empty() && std::all_of(str.begin(),str.end(),[](char ch)->bool
	{
		return ch>='0' && ch <='9';
	});
}


int main(int argc,const char *argv[])
{
	
	bool iswin,nonoecanwin,idquit=false;
	
	unsigned int winindex;

	
	/*********************************  Option  *********************************/
	unsigned int bid=BID,money=MONEY;
	unsigned int id,maxplayer=0;
	std::string str;
	group_t group;
	gen_t gen(system_clock::to_time_t(system_clock::now()));
	
	Copt opt(argc, argv, option);
	
	if (opt.argc == 1) {
		showhelp(option,option_des);
        return 0;
    }
	
    while (std::tie(id, str) = opt.action(), id != Copt::ID::END) {
        switch (id) {
        case optid::opt_b:
			
			if(!isdigital(str))
				return showerr(err_des,errid::err_nan, option[opt_b]+str);
			
            bid=std::stoul(str);
            break;

        case optid::opt_m:
		
			if(!isdigital(str))
				return showerr(err_des, errid::err_nan, option[opt_m]+str);
			
			money=std::stoul(str);
            break;

        case optid::opt_h:
			if(str.empty()) return showerr(err_des,errid::err_empty, option[opt_h]+str);
			group.push_back(player_ptr(new human(maxplayer++,str.c_str(),MONEY)));
	
            break;

        case optid::opt_c:
			if(str.empty()) return showerr(err_des,errid::err_empty, option[opt_c]+str);
			group.push_back(player_ptr(new computer(maxplayer++,str.c_str(),MONEY,gen)));
			
            break;
			
        case optid::opt_c2:
			if(str.empty()) return showerr(err_des,errid::err_empty, option[opt_c2]+str);
			group.push_back(player_ptr(new computer2(maxplayer++,str.c_str(),MONEY)));
			
            break;

        case optid::opt_H:
            showhelp(option,option_des);
            return 0;
            break;

        default:
		showhelp(option,option_des);
		return showerr(err_des,errid::err_invalidopt, str);
        }
    }
	
	{
		unsigned int i=0;
		bool error=false;
	
		if(bid<BID)
			{i+=showerr(err_des,errid::err_bid, std::to_string(bid) +" < " + std::to_string(BID) + "(BID)");error=true;}
		if(money<MONEY)
			{i+=showerr(err_des,errid::err_money, std::to_string(money)+" < " +std::to_string(MONEY)+ "(MONEY)");error=true;}
		if(bid>money)
			{i+=showerr(err_des,errid::err_bidmoney, std::to_string(bid) + "(bid) > " +std::to_string(money)+ "(money)");error=true;}
	
		if(maxplayer<2)
			{i+=showerr(err_des,errid::err_up2player, std::to_string(maxplayer) + " < 2");error=true;}
		if(maxplayer>MAXPLAYER)
			{i+=showerr(err_des,errid::err_player, std::to_string(maxplayer) + " > " +std::to_string(MAXPLAYER)+ "(MAXPLAYER)");error=true;}
		if(bid*DRAW>money)
			{i+=showerr(err_des,errid::err_biddrawmoney, std::to_string(bid) + "*" + std::to_string(DRAW) + "(DRAW) > " +std::to_string(money)+ "(money)");error=true;}
		
		if(error) return i;
	}
	
	for(auto &playerPtr:group)
		playerPtr->money=money;
	
	game52_t game52(bid);
	/*********************************  Option  *********************************/
	
	std::vector<unsigned int> vec;
	
	deck_t deck=constructdeck(rank,suit);

	auto itbase=group.begin();
	
	do{
		auto it=itbase;
		
		bool outloop=false;
		
		game52.shufflephase(deck,SHUFFLE,gen);
	
		game52.drawphase(group,deck,DRAW);
			
		showinfo(game52,group,deck,it,itbase);
	
		do
		{
	
		while(it!=group.end())
			{
				auto &player=**it;
				
				if(player.live && player.canbid && player.money>=bid)
				{
					char ch=player.bid(group,deck,gen);
			
					std::cout << player.name << ": " << ch << std::endl;
			
					if(ch==IDYES)
					{
						game52.draw(player,deck);
						std::cout << player.name << " ===> " << player.deck.back() << std::endl;
				
					}
					
					else if(ch==IDQUIT) { idquit=true;break; }
				
					else
					{
						player.canbid=false;
					}
					
					auto it2=findnext(it,group.begin(),group.end(),[](const player_ptr &p)->bool{
						return p->live && p->canbid;});

					showinfo(game52,group,deck,it2!=group.end()?it2:it,itbase);
			
					std::tie(nonoecanwin,vec)=game52.gameover(group);
			
					if(!(!nonoecanwin && !vec.size())) { outloop=true;break; }
				}
				
				if(++it==group.end()) it=group.begin();
			}
			
		} while(!outloop && !idquit);
		
	if(idquit) break;
	
	if(!nonoecanwin && group[vec.front()]->score<=SCORE)
	{
		auto &winner=*group[vec.front()];
		showwinner(winner,game52.money);
		winner.money+=game52.money;
	}
	else{
		std::cout << "\n========= All fuckin win!! =========\n";
		for(auto &playerPtr:group)
			if(playerPtr->live)
			{
				showwinner(*playerPtr,playerPtr->deck.size()*bid);
				playerPtr->money+=(playerPtr->deck.size()*bid);
			}
		}
		
	game52.update(group);
	game52.endphase(group,deck);
	
	std::tie(iswin,winindex)=game52.matchover(group);
	
	if(!iswin && !idquit) 
		itbase=findnext(itbase,group.begin(),group.end(),[](const player_ptr &p)->bool{
		return p->live ;
			});
	
	}while(!iswin && !idquit);
	
	if(!idquit) showwinnerofthematch(*group[winindex]);
	
	
	return 0;
}

static void showwinner(const player_t &player,unsigned int money)
{
	std::cout << "\n========= Winner is " << player.name << "[+" << money << "] =========\n";
}

static void showwinnerofthematch(const player_t &player)
{
	std::cout << "\n\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Winner of the match is " << player.name << "[" << player.money << "] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
}

template <typename It>
static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck,It it, It itbase)
{
	std::cout << "\nMain Deck: <" << deck.size() << ">\n"
	
			  << "Money: <" << game52.money << ">\n"
			  
			  << "BID: <" << game52.bid << ">\n"
			  
			  << "Base Index: <" << (*itbase)->name << ">\n"
			  
			  << "Current Index: <" << (*it)->name << ">\n\n"
			  
			  << group;
	
}


template <std::size_t M,std::size_t N>
static deck_t constructdeck(const rank_t (&rank)[M],const suit_t (&suit)[N])
{
	deck_t deck;
	
	for(unsigned int i=0,c=0;i<N;i++)
	{
		for(unsigned int j=0;j<M;j++)
		{
			deck.push_back({c++,rank[j],suit[i]});
		}
	}
		
		return deck;
}

static int showerr(const char** err_des,unsigned int id,const std::string &str)
{
	std::cerr << std::endl << str << std::endl
			  << "Error ID:" << id << " ====> " << err_des[id] << std::endl;
	return id+1;
}

template <std::size_t M>
static void showhelp(const char* (&opt)[M],const char* (&opt_des)[M])
{
	std::cerr << "\n\n";
	for(unsigned int i=0;i<M;i++)
		std::cerr << opt[i] << " =====> " << opt_des[i] << std::endl;
	
	std::cerr << "\nMinimum bid(BID) = " << BID << std::endl
			  << "Minimum money(MONEY) = " << MONEY << std::endl
			  << "Maximum player(MAXPLAYER) = " << MAXPLAYER << std::endl
			  << "First Draw(DRAW) = " << DRAW << std::endl
			  << "Goal Score(SCORE) = " << SCORE << std::endl << std::endl
			  
			  << "Example\n"
			  << "52-r " << opt[opt_c] << "Hwoy " << opt[opt_c] << "View\n"
			  << "52-r " << opt[opt_h] << "Hwoy " << opt[opt_c] << "View "<< opt[opt_c] << "Kung\n";
}


template <typename T,typename F>
static T findnext(T it,T begin, T end ,const F &f)
{
	it=std::find_if(it+1,end,f);
			
	if(it==end) it=std::find_if(begin,end,f);
			
	return it;
}


