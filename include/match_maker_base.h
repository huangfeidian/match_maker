#pragma once
#include "match_info.h"
#include <unordered_map>
#include <memory>

namespace spiritsaway::system::match_maker
{
	class match_maker_base
	{
	public:
		enum class basic_match_state
		{
			idle = 0,
			candidate_for_faction,
			in_result

		};
		const std::uint32_t m_faction_num;
		const std::uint32_t m_faction_team_sz;
		const std::uint32_t m_max_team_player_sz;
		const std::uint32_t m_min_team_player_sz;
		struct candidate_team : public team_info
		{
			std::uint32_t match_state = 0;
			std::uint64_t apply_ts = 0;
		};
	protected:
		std::unordered_map< std::string, std::uint32_t> m_sz_for_team;
		std::vector<std::vector< candidate_team>> m_teams_by_sz;
		std::uint64_t m_now_ts;
		
	protected:
		struct search_contex
		{
			std::uint32_t remain_capacity;
			std::uint32_t cur_team_sz;
			std::uint32_t last_choose_idx;
		};
		std::vector<search_contex> m_temp_select_teams;
		bool search_for_faction(const std::vector<std::vector< candidate_team*>>& team_ptrs_by_sz, std::vector<candidate_team*>& cur_faction_group, const std::uint32_t remain_capacity, const std::uint32_t cur_team_sz, const std::uint32_t last_choose_idx);
		bool search_for_faction_recursive(const std::vector<std::vector< candidate_team*>>& team_ptrs_by_sz, std::vector<candidate_team*>& cur_faction_group, const std::uint32_t remain_capacity, const std::uint32_t cur_team_sz, const std::uint32_t last_choose_idx);
		bool search_for_faction_non_recursive(const std::vector<std::vector< candidate_team*>>& team_ptrs_by_sz, std::vector<candidate_team*>& cur_faction_group,  std::uint32_t remain_capacity,  std::uint32_t cur_team_sz, const std::uint32_t last_choose_idx);
	public:
		match_maker_base(const std::uint32_t faction_num, const std::uint32_t faction_team_sz, const std::uint32_t min_team_player_sz, const std::uint32_t max_team_player_sz, std::uint64_t now_ts);
		virtual std::uint32_t add_candidate(const team_info& new_team);
		virtual bool remove_candidate(const std::string& tid);
		virtual std::vector<match_result> make_matchs() = 0;
		void set_now_ts(std::uint64_t now_ts)
		{
			m_now_ts = now_ts;
		}
		~match_maker_base();
	};
}