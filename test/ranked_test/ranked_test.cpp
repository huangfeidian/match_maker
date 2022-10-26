#include "naive_ranked_match_maker.h"
#include <cassert>
#include <iostream>
#include <chrono>
using namespace std;
using namespace spiritsaway::match_maker;

void unit_test_1()
{
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, 1, 1, 0, cur_config);
	std::vector<team_info> cur_teams;
	for (int i = 0; i < 5; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(i);
		team_1.players.push_back(player_info{ "p" + std::to_string(i) });
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
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, 3, 3, 0, cur_config);
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
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, 3, 3, 0, cur_config);
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
			assert(cur_result.size() == (i - 4) / 2);
		}

		for (int j = 0; j < i + 1; j++)
		{
			cur_match_maker.remove_candidate(cur_teams[j].tid);
		}
	}
}

void unit_test_4()
{
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, 5, 5, 0, cur_config);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	std::uint32_t team_counter = 0;
	std::uint64_t timer_counter = 0;
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
			cur_match_maker.set_now_ts(timer_counter++);
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

void unit_test_5()
{
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, 5, 5, 0, cur_config);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	std::uint32_t team_counter = 0;
	std::uint64_t timer_counter = 0;
	for (int i = 0; i < 10; i++)
	{
		team_info team_1;
		team_1.rank_score = 0;
		team_1.tid = "t" + std::to_string(team_counter++);
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		cur_teams.push_back(team_1);
	}

	for (int i = 0; i < cur_teams.size(); i++)
	{
		cur_match_maker.set_now_ts(timer_counter++);
		cur_match_maker.add_candidate(cur_teams[i]);
	}
	auto cur_result = cur_match_maker.make_matchs();
	std::cout << "result sz is " << cur_result.size() << std::endl;
}


void unit_test_6(std::uint32_t faction_team_sz, std::uint32_t max_candidate_team_sz, std::uint32_t team_number)
{
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, faction_team_sz, max_candidate_team_sz, 0, cur_config);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	std::uint32_t team_counter = 0;
	std::uint64_t timer_counter = 0;

	for (int i = 0; i < max_candidate_team_sz; i++)
	{
		for (int j = 0; j < team_number; j++)
		{
			team_info team_1;
			team_1.rank_score = 0;
			team_1.tid = "t" + std::to_string(team_counter++);
			for (int k = 0; k <= i; k++)
			{
				team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
			}

			cur_teams.push_back(team_1);
		}
	}

	for (int i = 0; i < cur_teams.size(); i++)
	{
		cur_match_maker.set_now_ts(timer_counter++);
		cur_match_maker.add_candidate(cur_teams[i]);

	}
	auto begin_ts = std::chrono::system_clock::now();
	auto cur_match_result = cur_match_maker.make_matchs();
	auto end_ts = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end_ts - begin_ts;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s for faction_team_sz " << faction_team_sz << " max_candidate_sz " << max_candidate_team_sz << " team_numbers " << team_number << " result sz " << cur_match_result.size() << std::endl;;
}

void unit_test_7()
{
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, 5, 5, 0, cur_config);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	std::uint32_t team_counter = 0;
	std::uint64_t timer_counter = 0;
	for (int i = 0; i < 10; i++)
	{
		team_info team_1;
		team_1.rank_score = i*10;
		team_1.tid = "t" + std::to_string(team_counter++);
		team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
		cur_teams.push_back(team_1);
	}

	for (int i = 0; i < cur_teams.size(); i++)
	{
		cur_match_maker.set_now_ts(timer_counter++);
		cur_match_maker.add_candidate(cur_teams[i]);
	}
	auto cur_result = cur_match_maker.make_matchs();
	assert(cur_result.size() == 0);

	cur_match_maker.set_now_ts(303);
	cur_result = cur_match_maker.make_matchs();
	assert(cur_result.size() == 0);

	cur_match_maker.set_now_ts(304);
	cur_result = cur_match_maker.make_matchs();
	assert(cur_result.size() == 1);

	std::cout << "result sz is " << cur_result.size() << std::endl;
}

void unit_test_8(std::uint32_t faction_team_sz, std::uint32_t max_candidate_team_sz, std::uint32_t team_number)
{
	naive_ranked_match_maker::ranked_match_config cur_config{ 50, 300, 2 };
	naive_ranked_match_maker cur_match_maker(2, faction_team_sz, max_candidate_team_sz, 0, cur_config);
	std::vector<team_info> cur_teams;
	std::uint32_t player_counter = 0;
	std::uint32_t team_counter = 0;
	std::uint64_t timer_counter = 0;

	for (int i = 0; i < max_candidate_team_sz; i++)
	{
		for (int j = 0; j < team_number; j++)
		{
			team_info team_1;
			team_1.rank_score = team_counter /30;
			team_1.tid = "t" + std::to_string(team_counter++);
			for (int k = 0; k <= i; k++)
			{
				team_1.players.push_back(player_info{ "p" + std::to_string(player_counter++) });
			}

			cur_teams.push_back(team_1);
		}
	}

	for (int i = 0; i < cur_teams.size(); i++)
	{
		cur_match_maker.set_now_ts(timer_counter++);
		cur_match_maker.add_candidate(cur_teams[i]);

	}
	for (int i = 0; i < 6; i++)
	{
		cur_match_maker.set_now_ts(timer_counter + i * 100);
		auto begin_ts = std::chrono::system_clock::now();
		auto cur_match_result = cur_match_maker.make_matchs();
		auto end_ts = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end_ts - begin_ts;
		std::cout << "elapsed time: " << elapsed_seconds.count() << "s for faction_team_sz " << faction_team_sz << " max_candidate_sz " << max_candidate_team_sz << " team_numbers " << team_number << " result sz " << cur_match_result.size() << std::endl;;
	}
	
}

int main()
{
	unit_test_1();
	unit_test_2();
	unit_test_3();
	unit_test_4();
	unit_test_5();
	unit_test_6(5, 3, 10000);
	unit_test_6(10, 3, 10000);
	unit_test_6(20, 3, 10000);
	unit_test_6(43, 3, 10000);
	unit_test_7();
	unit_test_8(5, 3, 10000);
	unit_test_8(10, 3, 10000);
	unit_test_8(20, 3, 10000);
	unit_test_8(43, 3, 10000);
}