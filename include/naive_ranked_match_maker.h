#pragma once
#include "match_maker_base.h"
namespace spiritsaway::system::match_maker
{
	class naive_ranked_match_maker: public match_maker_base
	{
	public:
		struct ranked_match_config
		{
			float rank_level_gap_score;
			std::uint64_t extend_level_tolerance_time_gap;
			std::uint32_t max_level_diff_tolerance;
		};
	protected:
		const ranked_match_config m_config;
	public:
		naive_ranked_match_maker(const std::uint32_t faction_num, const std::uint32_t faction_team_sz, const std::uint32_t min_team_player_sz, const std::uint32_t max_team_player_sz, std::uint64_t now_ts, const ranked_match_config& match_config);
		std::vector<match_result> make_matchs() override;
	};
}
