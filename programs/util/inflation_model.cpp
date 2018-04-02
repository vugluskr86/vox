
#include <steemit/chain/compound.hpp>
#include <steemit/protocol/asset.hpp>
#include <steemit/protocol/types.hpp>
#include <fc/io/json.hpp>
#include <fc/variant_object.hpp>

#include <iostream>
#include <vector>

#define CONTENT      0
#define PRODUCER     1
#define VESTING      2
#define LIQUIDITY    3
#define REWARD_TYPES 4

using steemit::protocol::asset;
using steemit::protocol::share_type;
using steemit::protocol::calc_percent_reward_per_block;
using steemit::protocol::calc_percent_reward_per_round;
using steemit::protocol::calc_percent_reward_per_hour;

/*
Explanation of output

{"rvec":["929159090641","8360617424769","929159090641","8360617424769","197985103985","1780051544865","195077031513","1755693283617","179687790278","1615357001502"],"b":68585000,"s":"24303404786580"}

rvec shows total number of STEEM satoshis created since genesis for:

- Curation rewards
- Vesting rewards balancing curation rewards
- Content rewards
- Vesting rewards balancing content rewards
- Producer rewards
- Vesting rewards balancing producer rewards
- Liquidity rewards
- Vesting rewards balancing liquidity rewards

b is block number
s is total supply


Some possible sources of inaccuracy, the direction and estimated relative sizes of these effects:

- Missed blocks not modeled (lowers STEEM supply, small)
- Miner queue length very approximately modeled (assumed to go to 100 during the first blocks and then stay there) (may lower or raise STEEM supply, very small)
- Creation / destruction of STEEM used to back SBD not modeled (moves STEEM supply in direction opposite to changes in dollar value of 1 STEEM, large)
- Interest paid to SBD not modeled (raises STEEM supply, medium)
- Lost / forgotten private keys / wallets and deliberate burning of STEEM not modeled (lowers STEEM supply, unknown but likely small)
- Possible bugs or mismatches with implementation (unknown)

*/

int main( int argc, char** argv, char** envp )
{
   std::vector< share_type > reward_delta;
   std::vector< share_type > reward_total;

   uint32_t liquidity_begin_block = STEEMIT_START_VESTING_BLOCK;
   
   for( int i=0; i<REWARD_TYPES; i++ )
   {
      reward_delta.emplace_back();
      reward_total.emplace_back();
   }

   auto block_inflation_model = [&]( uint32_t block_num, share_type& current_supply )
   {
      int64_t start_inflation_rate = int64_t( STEEMIT_INFLATION_RATE_START_PERCENT );
      int64_t inflation_rate_adjustment = int64_t( block_num / STEEMIT_INFLATION_NARROWING_PERIOD );
      int64_t inflation_rate_floor = int64_t( STEEMIT_INFLATION_RATE_STOP_PERCENT );

      // below subtraction cannot underflow int64_t because inflation_rate_adjustment is <2^32
      int64_t current_inflation_rate = std::max( start_inflation_rate - inflation_rate_adjustment, inflation_rate_floor );

      auto new_steem = ( current_supply * current_inflation_rate ) / ( int64_t( STEEMIT_100_PERCENT ) * int64_t( STEEMIT_BLOCKS_PER_YEAR ) );
      
      reward_delta[CONTENT]  = ( new_steem * STEEMIT_CONTENT_REWARD_PERCENT ) / STEEMIT_100_PERCENT;

      reward_delta[VESTING]  = ( new_steem * STEEMIT_VESTING_FUND_PERCENT ) / STEEMIT_100_PERCENT; /// 15% to vesting fund
      
      reward_delta[PRODUCER] =  new_steem - reward_delta[CONTENT] - reward_delta[VESTING]; /// Remaining 10% to witness pay


      current_supply += reward_delta[CONTENT] + reward_delta[VESTING] + reward_delta[PRODUCER];
      

      share_type liquidity_reward = 0;
      

      // if( (block_num > liquidity_begin_block) && (block_num < STEEMIT_LIQUIDITY_REWARD_BLOCKS))
      // {
      //    liquidity_reward = calc_percent_reward_per_hour< STEEMIT_LIQUIDITY_APR_PERCENT >( current_supply );
      //    liquidity_reward = std::max( liquidity_reward, STEEMIT_MIN_LIQUIDITY_REWARD.amount );
      // }
      // reward_delta[ LIQUIDITY] = liquidity_reward;
      // current_supply += reward_delta[ LIQUIDITY];



      for( int i=0; i<REWARD_TYPES; i++ )
      {
         reward_total[i] += reward_delta[i];
      }

      return;

   };



   share_type current_supply = STEEMIT_INIT_SUPPLY;


   for( uint32_t b=1; b<1*STEEMIT_BLOCKS_PER_YEAR; b++ )
   {
      block_inflation_model( b, current_supply );
      if( b%1000 == 0 )
      {
         fc::mutable_variant_object mvo;
         mvo("rvec", reward_total)("b", b)("s", current_supply);
         std::cout << fc::json::to_string(mvo) << std::endl;
      }
   }

   return 0;
}
