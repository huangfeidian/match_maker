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
	bool match_maker_base::search_for_faction_recursive(const std::vector<std::vector< candidate_team*>>& team_ptrs_by_sz, std::vector<candidate_team*>& cur_faction_group, const std::uint32_t remain_capacity, const std::uint32_t cur_team_sz, const std::uint32_t last_choose_idx)
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
				cur_faction_group.push_back(cur_team_ptr);
				if (search_for_faction_recursive(team_ptrs_by_sz, cur_faction_group, remain_capacity - cur_team_sz, cur_team_sz, i))
				{
					return true;
				}
				else
				{
					cur_faction_group.pop_back();
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
				cur_faction_group.push_back(cur_team_ptr);
				if (search_for_faction_recursive(team_ptrs_by_sz, cur_faction_group, remain_capacity - i, i, j ))
				{
					
					return true;
				}
				else
				{
					cur_faction_group.pop_back();
					cur_team_ptr->match_state = std::uint32_t(basic_match_state::idle);
					break;
				}
			}

		}
		return false;

	}

	bool match_maker_base::search_for_faction_non_recursive(const std::vector<std::vector< candidate_team*>>& team_ptrs_by_sz, std::vector<candidate_team*>& cur_faction_group,  std::uint32_t begin_remain_capacity,  std::uint32_t begin_cur_team_sz, const std::uint32_t begin_last_choose_idx)
	{
		if (begin_remain_capacity == 0)
		{
			return true;
		}
		m_temp_select_teams.clear();
		m_temp_select_teams.push_back(search_contex{ begin_remain_capacity, begin_cur_team_sz, begin_last_choose_idx });
		while (true)
		{
			
			auto last_contex = m_temp_select_teams.back();
			if (last_contex.remain_capacity == 0)
			{
				for (int i = 1; i < m_temp_select_teams.size(); i++)
				{
					cur_faction_group.push_back(team_ptrs_by_sz[m_temp_select_teams[i].cur_team_sz][m_temp_select_teams[i].last_choose_idx]);
				}
				return true;
			}
			bool has_choose_next = false;
			if (last_contex.remain_capacity >= last_contex.cur_team_sz)
			{
				for (std::uint32_t i = last_contex.last_choose_idx + 1; i < team_ptrs_by_sz[last_contex.cur_team_sz].size(); i++)
				{
					auto cur_team_ptr = team_ptrs_by_sz[last_contex.cur_team_sz][i];
					if (cur_team_ptr->match_state != std::uint32_t(basic_match_state::idle))
					{
						continue;
					}
					cur_team_ptr->match_state = std::uint32_t(basic_match_state::candidate_for_faction);
					m_temp_select_teams.push_back(search_contex{ last_contex.remain_capacity - last_contex.cur_team_sz, last_contex.cur_team_sz, i });
					has_choose_next = true;
					break;

				}
			}
			if (has_choose_next)
			{
				continue;
			}
			while (!has_choose_next)
			{
				
				for (std::uint32_t i = std::min(last_contex.cur_team_sz - 1, last_contex.remain_capacity); i > 0; i--)
				{
					for (std::uint32_t j = 0; j < team_ptrs_by_sz[i].size(); j++)
					{
						auto cur_team_ptr = team_ptrs_by_sz[i][j];
						if (cur_team_ptr->match_state != std::uint32_t(basic_match_state::idle))
						{
							continue;
						}
						cur_team_ptr->match_state = std::uint32_t(basic_match_state::candidate_for_faction);
						m_temp_select_teams.push_back(search_contex{ last_contex.remain_capacity - i, i, j });
						has_choose_next = true;
						break;

					}
					if (has_choose_next)
					{
						break;
					}
					

				}
				if (!has_choose_next)
				{
					// 如果还是找不到 则pop出上一个选择的
					if (m_temp_select_teams.size() == 1)
					{
						return false;
					}
					last_contex = m_temp_select_teams.back();
					last_contex.remain_capacity += last_contex.cur_team_sz;
					team_ptrs_by_sz[last_contex.cur_team_sz][last_contex.last_choose_idx]->match_state = std::uint32_t(basic_match_state::idle);
					m_temp_select_teams.pop_back();


				}
				else
				{
					break;
				}
			}


			

		}



		
		return false;

	}
	bool match_maker_base::search_for_faction(const std::vector<std::vector< candidate_team*>>& team_ptrs_by_sz, std::vector<candidate_team*>& cur_faction_group, const std::uint32_t remain_capacity, const std::uint32_t cur_team_sz, const std::uint32_t last_choose_idx)
	{
		// return search_for_faction_non_recursive(team_ptrs_by_sz, cur_faction_group, remain_capacity, cur_team_sz, last_choose_idx);
		return search_for_faction_recursive(team_ptrs_by_sz, cur_faction_group, remain_capacity, cur_team_sz, last_choose_idx);
	}
}