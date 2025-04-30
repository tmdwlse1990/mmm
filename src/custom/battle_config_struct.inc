// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config structure
 * Place the definition of custom battle configuration structure members in this file.
 *
 * Make sure that the name you want is not already taken by looking into battle.hpp
 * Do not forget to add initilization logic to battle_config_init.inc
 *
 * Format:
 *	 <datatype> name;
 **/


 int32 config_maxlv;
 int32 config_global_mvp_drop;
 int32 config_global_mvp_drop_rate;
 int32 mob_mvp_skill_rate;
 int32 max_sell_item_cap;
 int32 max_seal_item_val;
 int32 config_equipment_drop_auto_identify;
 int32 config_random_monster_rank;
 int32 config_monster_rank_damage_mode;
 int32 config_monster_rank_defense_mode;
 int32 config_monster_rank_exp_bonus_mode;
 int32 config_monster_rank_drop_bonus_mode;
 int32 config_enchantment_clickable;
 int32 config_enchantment_maximum;
 // auto attack
 int32 autoattack_skillheal;
 int32 autoattack_item_potion;
 int32 autoattack_sittorest;
 int32 autoattack_skill_attack;
 int32 autoattack_skill_buff;
 int32 autoattack_item_buff;
 int32 autoattack_teleport;
 int32 autoattack_item_pickup;
 int32 autoattack_mselection;
 int32 autoattack_item_range_detection;
 int32 autoattack_mob_detection;
 int32 autoattack_move;
 int32 autoattack_move_type;
 int32 autoattack_interval_timer;
 int32 autoattack_pickup_delay;
 int32 autoattack_clean_logout;
 int32 autoattack_hateffect;
 int32 autoattack_skill_rate_default;
 int32 autoattack_reduce_droprate;
 int32 autoattack_reduce_mode;
 int32 autoattack_max_moblist;
 int32 autoattack_max_itemlist;
 int32 autoattack_steal_reduce_rate;
 int32 autoattack_max_time_per_mob;
 int32 autoattack_mob_struck;
 int32 autoattack_quiver_check;
 int32 autoattack_max_flee_mob;
 int32 autoattack_gunslinger_conflip_fix;
 int32 autoattack_coinflip_coin;