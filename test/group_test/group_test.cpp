#include "naive_set_match_maker.h"
#include <cassert>
using namespace std;
using namespace spiritsaway::match_maker;

void unit_test_1()
{
	naive_set_match_maker cur_match_maker(2, 1, 1, 0);
	std::vector<team_info> cur_teams;
	for (int i = 0; i < 5; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(i);
		team_1.players.push_back(player_info{ "p" + std::to_string(i)});
		cur_teams.push_back(team_1);
	}
	
	for (int i = 0; i < cur_teams.size(); i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.add_candidate(cur_teams[j]);
		}
		auto cur_result = cur_match_maker.make_matchs();
		assert(cur_result.size() == (i + 1) / 2);
		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.remove_candidate(cur_teams[j].tid);
		}
	}
}

void unit_test_2()
{
	naive_set_match_maker cur_match_maker(2, 3, 3, 0);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	for (int i = 0; i < 5; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(i);
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		cur_teams.push_back(team_1);
	}

	for (int i = 0; i < cur_teams.size(); i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.add_candidate(cur_teams[j]);
		}
		auto cur_result = cur_match_maker.make_matchs();
		assert(cur_result.size() == 0);
		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.remove_candidate(cur_teams[j].tid);
		}
	}
}
void unit_test_3()
{
	naive_set_match_maker cur_match_maker(2, 3, 3, 0);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	std::uint32_t team_counter = 0;
	for (int i = 0; i < 5; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(team_counter++);
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		cur_teams.push_back(team_1);
	}
	for (int i = 0; i < 5; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(team_counter++);
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		cur_teams.push_back(team_1);
	}
	for (int i = 0; i < cur_teams.size(); i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.add_candidate(cur_teams[j]);
		}
		auto cur_result = cur_match_maker.make_matchs();
		if (i < 6)
		{
			assert(cur_result.size() == 0);
		}
		else
		{
			assert(cur_result.size() == (i -4)/2);
		}
		
		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.remove_candidate(cur_teams[j].tid);
		}
	}
}

void unit_test_4()
{
	naive_set_match_maker cur_match_maker(2, 5, 5, 0);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	std::uint32_t team_counter = 0;
	std::uint64_t timer_counter = 1000;
	for (int i = 0; i < 5; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(team_counter++);
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		cur_teams.push_back(team_1);
	}
	for (int i = 0; i < 5; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(team_counter++);
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		cur_teams.push_back(team_1);
	}
	for (int i = 0; i < cur_teams.size(); i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.set_now_ts(timer_counter--);
			cur_match_maker.add_candidate(cur_teams[j]);
			
		}
		auto cur_result = cur_match_maker.make_matchs();
		if (i < 6)
		{
			assert(cur_result.size() == 0);
		}
		else
		{
			assert(cur_result.size() == std::min(1, (i - 4) / 2));
		}

		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.remove_candidate(cur_teams[j].tid);
		}
	}
}

int main()
{
	unit_test_1();
	unit_test_2();
	unit_test_3();
	unit_test_4();
}