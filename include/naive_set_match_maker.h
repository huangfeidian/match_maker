#pragma once
#include "match_maker_base.h"
namespace spiritsaway::system::match_maker
{
	class naive_set_match_maker : public match_maker_base
	{

	public:
		using match_maker_base::match_maker_base;
		std::vector<match_result> make_matchs() override;
	};
}
