#include "match_maker_base.h"

namespace spiritsaway::match_maker
{
	match_maker_base::match_maker_base(const std::uint32_t faction_num, const std::uint32_t faction_team_sz, const std::uint32_t max_team_player_sz, std::uint64_t now_ts)
		: m_faction_num(faction_num)
		, m_faction_team_sz(faction_team_sz)
		, m_max_team_player_sz(max_team_player_sz)
		, m_teams_by_sz(max_team_player_sz + 1)
		, m_now_ts(now_ts)
	{

	}

	void match_maker_base::add_candidate(const team_info& new_team)
	{
		if (new_team.players.size() > m_max_team_player_sz)
		{
			return;
		}
		if (new_team.players.empty())
		{
			return;
		}
		auto temp_iter = m_sz_for_team.find(new_team.tid);
		if (temp_iter != m_sz_for_team.end())
		{
			return;
		}
		m_sz_for_team[new_team.tid] = new_team.players.size();
		m_teams_by_sz[new_team.players.size()].push_back(candidate_team{ new_team, 0 , m_now_ts });
	}

	void match_maker_base::remove_candidate(const std::string& tid)
	{
		auto cur_iter = m_sz_for_team.find(tid);
		if (cur_iter == m_sz_for_team.end())
		{
			return;
		}
		auto& cur_team_vec = m_teams_by_sz[cur_iter->second];
		for (std::uint32_t i = 0; i < cur_team_vec.size(); i++)
		{
			if (cur_team_vec[i].tid == tid)
			{
				if (i + 1 == cur_team_vec.size())
				{
					cur_team_vec.pop_back();
				}
				else
				{
					std::swap(cur_team_vec[i], cur_team_vec.back());
					cur_team_vec.pop_back();
				}
				break;
			}
		}
		m_sz_for_team.erase(cur_iter);

	}
	match_maker_base::~match_maker_base()
	{

	}
	bool match_maker_base::search_for_faction(std::vector<std::vector< candidate_team*>> team_ptrs_by_sz, std::vector<candidate_team*>& cur_faction_group, const std::uint32_t remain_capacity, const std::uint32_t cur_team_sz, const std::uint32_t last_choose_idx)
	{
		if (remain_capacity == 0)
		{
			return true;
		}
		if (remain_capacity >= cur_team_sz)
		{
			for (std::uint32_t i = last_choose_idx + 1; i < team_ptrs_by_sz[cur_team_sz].size(); i++)
			{
				auto cur_team_ptr = team_ptrs_by_sz[cur_team_sz][i];
				if (cur_team_ptr->match_state != std::uint32_t(basic_match_state::idle))
				{
					continue;
				}
				cur_team_ptr->match_state = std::uint32_t(basic_match_state::candidate_for_faction);

				if (search_for_faction(team_ptrs_by_sz, cur_faction_group, remain_capacity - cur_team_sz, cur_team_sz, i))
				{
					cur_faction_group.push_back(cur_team_ptr);
					return true;
				}
				else
				{
					cur_team_ptr->match_state = std::uint32_t(basic_match_state::idle);
					break;
				}

			}
		}


		for (std::uint32_t i = std::min(cur_team_sz - 1, remain_capacity); i > 0; i--)
		{
			for (std::uint32_t j = 0; j < team_ptrs_by_sz[i].size(); j++)
			{
				auto cur_team_ptr = team_ptrs_by_sz[i][j];
				if (cur_team_ptr->match_state != std::uint32_t(basic_match_state::idle))
				{
					continue;
				}
				cur_team_ptr->match_state = std::uint32_t(basic_match_state::candidate_for_faction);
				if (search_for_faction(team_ptrs_by_sz, cur_faction_group, remain_capacity - i, i, j ))
				{
					cur_faction_group.push_back(cur_team_ptr);
					return true;
				}
				else
				{
					cur_team_ptr->match_state = std::uint32_t(basic_match_state::idle);
					break;
				}
			}

		}
		return false;

	}
}