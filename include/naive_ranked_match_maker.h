#pragma once
#include "match_maker_base.h"
namespace spiritsaway::system::match_maker
{
	struct ranked_match_config
	{
		float rank_level_gap_score;
		std::uint64_t extend_level_tolerance_time_gap;
		std::uint32_t max_level_diff_tolerance;
	};
	class naive_ranked_match_maker: public match_maker_base
	{
	public:
		
	protected:
		const ranked_match_config m_ranked_config;
	public:
		naive_ranked_match_maker(const match_base_config& in_base_config, std::uint64_t now_ts, const ranked_match_config& in_ranked_config);
		std::vector<match_result> make_matchs() override;
	};
}
