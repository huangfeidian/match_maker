#include "naive_ranked_match_maker.h"
#include <algorithm>
namespace spiritsaway::match_maker
{
	naive_ranked_match_maker::naive_ranked_match_maker(const std::uint32_t faction_num, const std::uint32_t faction_team_sz, const std::uint32_t max_team_player_sz, std::uint64_t now_ts, const ranked_match_config& match_config)
		: match_maker_base(faction_num, faction_team_sz, max_team_player_sz, now_ts)
		, m_config(match_config)
	{

	}

	std::vector<match_result> naive_ranked_match_maker::make_matchs()
	{
		float min_score, max_score;
		bool score_set = false;
		for (const auto& one_team_vec : m_teams_by_sz)
		{
			for (const auto& one_team : one_team_vec)
			{
				if (!score_set)
				{
					score_set = true;
					min_score = one_team.rank_score;
					max_score = one_team.rank_score;
				}
				else
				{
					min_score = std::min(min_score, one_team.rank_score);
					max_score = std::max(max_score, one_team.rank_score);
				}
				
			}
		}
		if (!score_set)
		{
			return {};
		}

		int min_score_level = int(std::floor(min_score / m_config.rank_level_gap_score));
		int max_score_level = int(std::ceil(max_score / m_config.rank_level_gap_score));
		min_score = min_score_level * m_config.rank_level_gap_score;
		max_score = max_score_level * m_config.rank_level_gap_score;
		std::uint32_t level_range_sz = max_score_level - min_score_level + 1;
		std::vector<std::vector<std::vector< candidate_team*>>> team_ptrs_by_sz_and_level(level_range_sz, std::vector<std::vector< candidate_team*>>(m_teams_by_sz.size()));
		std::vector<match_result> final_match_results;
		for (std::uint32_t i = 1; i < m_teams_by_sz.size(); i++)
		{
			std::sort(m_teams_by_sz[i].begin(), m_teams_by_sz[i].end(), [](const candidate_team& team_a, const candidate_team& team_b)
				{
					return team_a.apply_ts < team_b.apply_ts;
				});

			for (std::uint32_t j = 0; j < m_teams_by_sz[i].size(); j++)
			{
				candidate_team* cur_team_ptr = &m_teams_by_sz[i][j];
				auto cur_team_match_level_tolerance = int((m_now_ts - cur_team_ptr->apply_ts) / m_config.extend_level_tolerance_time_gap);
				cur_team_match_level_tolerance = std::min(cur_team_match_level_tolerance, int(m_config.max_level_diff_tolerance));
				int cur_team_match_level = int(cur_team_ptr->rank_score / m_config.rank_level_gap_score);
				for (int k = std::max(min_score_level, cur_team_match_level - cur_team_match_level_tolerance); k <= std::min(max_score_level, cur_team_match_level + cur_team_match_level_tolerance); k++)
				{
					auto& cur_team_vec = team_ptrs_by_sz_and_level[k - min_score_level][cur_team_ptr->players.size()];
					if (cur_team_vec.capacity() < 8)
					{
						cur_team_vec.reserve(8);
					}
					cur_team_vec.push_back(cur_team_ptr);
				}
			}
		}

		for (std::uint32_t k = 0; k < team_ptrs_by_sz_and_level.size(); k++)
		{
			std::vector<std::vector<candidate_team*>> faction_result;
			std::vector<candidate_team*> temp_faction;
			auto& team_ptrs_by_sz = team_ptrs_by_sz_and_level[k];
			for (std::uint32_t i = team_ptrs_by_sz.size() - 1; i > 0; i--)
			{

				for (std::uint32_t j = 0; j < team_ptrs_by_sz[i].size(); j++)
				{

					if (team_ptrs_by_sz[i][j]->match_state != std::uint32_t(basic_match_state::idle))
					{
						continue;
					}
					temp_faction.clear();
					team_ptrs_by_sz[i][j]->match_state = std::uint32_t(basic_match_state::candidate_for_faction);
					if (search_for_faction(team_ptrs_by_sz, temp_faction, m_faction_team_sz - i, i, j))
					{
						faction_result.push_back({});
						temp_faction.push_back(team_ptrs_by_sz[i][j]);
						for (auto one_team : temp_faction)
						{
							one_team->match_state = std::uint32_t(basic_match_state::in_result);
							faction_result.back().push_back(one_team);
						}
					}
					else
					{
						team_ptrs_by_sz[i][j]->match_state = std::uint32_t(basic_match_state::idle);
						break;
					}
				}
			}
			std::vector<match_result> match_results(faction_result.size() / m_faction_num);
			for (std::uint32_t i = 0; i < match_results.size(); i++)
			{
				match_results[i].factions.resize(m_faction_num);
				for (std::uint32_t j = 0; j < m_faction_num; j++)
				{
					match_results[i].factions[j].teams.reserve(faction_result[i * m_faction_num + j].size());
					for (auto one_team_ptr : faction_result[i * m_faction_num + j])
					{
						match_results[i].factions[j].teams.push_back(*one_team_ptr);
						m_sz_for_team.erase(one_team_ptr->tid);
					}

				}
			}
			for (std::uint32_t i = match_results.size() * m_faction_num; i < faction_result.size(); i++)
			{
				for (auto one_team_ptr : faction_result[i])
				{
					one_team_ptr->match_state = std::uint32_t(basic_match_state::idle);
				}
			}
			final_match_results.insert(final_match_results.end(), match_results.begin(), match_results.end());
		}

		std::vector<candidate_team> remain_teams;
		remain_teams.reserve(8);
		for (std::uint32_t i = 1; i < m_teams_by_sz.size(); i++)
		{
			if (m_teams_by_sz[i].empty())
			{
				continue;
			}
			remain_teams.clear();
			for (const auto& one_team : m_teams_by_sz[i])
			{
				if (one_team.match_state == std::uint32_t(basic_match_state::idle))
				{
					remain_teams.push_back(one_team);
				}
			}
			std::swap(remain_teams, m_teams_by_sz[i]);
		}
		return final_match_results;





	}
}