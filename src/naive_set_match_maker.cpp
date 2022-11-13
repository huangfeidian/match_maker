#include "naive_set_match_maker.h"
#include <algorithm>
namespace spiritsaway::system::match_maker
{


	std::vector<match_result> naive_set_match_maker::make_matchs()
	{
		std::vector<std::vector<candidate_team*>> faction_result;
		std::vector<candidate_team*> temp_faction;
		std::vector<std::vector< candidate_team*>> team_ptrs_by_sz(m_teams_by_sz.size());
		for (std::uint32_t i = 1; i < team_ptrs_by_sz.size(); i++)
		{
			std::sort(m_teams_by_sz[i].begin(), m_teams_by_sz[i].end(), [](const candidate_team& team_a, const candidate_team& team_b)
				{
					return team_a.apply_ts < team_b.apply_ts;
				});
			team_ptrs_by_sz[i].resize(m_teams_by_sz[i].size());
			for (std::uint32_t j = 0; j < m_teams_by_sz[i].size(); j++)
			{
				team_ptrs_by_sz[i][j] = m_teams_by_sz[i].data() + j;
			}
		}
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
				temp_faction.push_back(team_ptrs_by_sz[i][j]);
				if (search_for_faction(team_ptrs_by_sz, temp_faction, m_base_config.faction_team_sz - i, i, j))
				{
					faction_result.push_back({});
					faction_result.back().reserve(temp_faction.size());
					for (auto one_team : temp_faction)
					{
						one_team->match_state = std::uint32_t(basic_match_state::in_result);
						faction_result.back().push_back(one_team);
					}
				}
				else
				{
					temp_faction.pop_back();
					team_ptrs_by_sz[i][j]->match_state = std::uint32_t(basic_match_state::idle);
					break;
				}
			}
		}
		std::vector<match_result> match_results(faction_result.size() / m_base_config.faction_num);
		for (std::uint32_t i = 0; i < match_results.size(); i++)
		{
			match_results[i].factions.resize(m_base_config.faction_num);
			for (std::uint32_t j = 0; j < m_base_config.faction_num; j++)
			{
				match_results[i].factions[j].teams.reserve(faction_result[i * m_base_config.faction_num + j].size());
				for (auto one_team_ptr : faction_result[i * m_base_config.faction_num + j])
				{
					match_results[i].factions[j].teams.push_back(*one_team_ptr);
					m_sz_for_team.erase(one_team_ptr->tid);
				}
				
			}
		}
		for (std::uint32_t i = match_results.size() * m_base_config.faction_num; i < faction_result.size(); i++)
		{
			for (auto one_team_ptr : faction_result[i])
			{
				one_team_ptr->match_state = std::uint32_t(basic_match_state::idle);
			}
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

		return match_results;

	}
}