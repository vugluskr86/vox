#include <boost/container/flat_map.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <steemit/manifest/plugins.hpp>

namespace steemit { namespace plugin {


std::shared_ptr< steemit::app::abstract_plugin > create_raw_block_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_auth_util_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_blockchain_statistics_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_witness_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_private_message_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_account_history_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_follow_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_account_statistics_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_debug_node_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_block_info_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_tags_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_delayed_node_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_account_by_key_plugin( steemit::app::application* app );

std::shared_ptr< steemit::app::abstract_plugin > create_market_history_plugin( steemit::app::application* app );


boost::container::flat_map< std::string, std::function< std::shared_ptr< steemit::app::abstract_plugin >( steemit::app::application* app ) > > plugin_factories_by_name;

void initialize_plugin_factories()
{
   
   plugin_factories_by_name[ "raw_block" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_raw_block_plugin( app );
   };
   
   plugin_factories_by_name[ "auth_util" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_auth_util_plugin( app );
   };
   
   plugin_factories_by_name[ "blockchain_statistics" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_blockchain_statistics_plugin( app );
   };
   
   plugin_factories_by_name[ "witness" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_witness_plugin( app );
   };
   
   plugin_factories_by_name[ "private_message" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_private_message_plugin( app );
   };
   
   plugin_factories_by_name[ "account_history" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_account_history_plugin( app );
   };
   
   plugin_factories_by_name[ "follow" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_follow_plugin( app );
   };
   
   plugin_factories_by_name[ "account_statistics" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_account_statistics_plugin( app );
   };
   
   plugin_factories_by_name[ "debug_node" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_debug_node_plugin( app );
   };
   
   plugin_factories_by_name[ "block_info" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_block_info_plugin( app );
   };
   
   plugin_factories_by_name[ "tags" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_tags_plugin( app );
   };
   
   plugin_factories_by_name[ "delayed_node" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_delayed_node_plugin( app );
   };
   
   plugin_factories_by_name[ "account_by_key" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_account_by_key_plugin( app );
   };
   
   plugin_factories_by_name[ "market_history" ] = []( steemit::app::application* app ) -> std::shared_ptr< steemit::app::abstract_plugin >
   {
      return create_market_history_plugin( app );
   };
   
}

std::shared_ptr< steemit::app::abstract_plugin > create_plugin( const std::string& name, steemit::app::application* app )
{
   auto it = plugin_factories_by_name.find( name );
   if( it == plugin_factories_by_name.end() )
      return std::shared_ptr< steemit::app::abstract_plugin >();
   return it->second( app );
}

std::vector< std::string > get_available_plugins()
{
   std::vector< std::string > result;
   for( const auto& e : plugin_factories_by_name )
      result.push_back( e.first );
   return result;
}

} }