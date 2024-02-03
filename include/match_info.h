#pragma once
#include <string>
#include <vector>
#include <cstdint>
namespace spiritsaway::system::match_maker
{
	struct player_info
	{
		std::string pid;
	};
	struct team_info
	{
		std::vector<player_info> players;
		float rank_score;
		std::string tid;
	};
	struct faction_group
	{
		std::vector<team_info> teams;
	};
	struct match_result
	{
		std::vector<faction_group> factions;
	};
}
