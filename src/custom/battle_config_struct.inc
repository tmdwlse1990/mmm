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


 int config_maxlv;
 int config_global_mvp_drop;
 int config_global_mvp_drop_rate;
 int mob_mvp_skill_rate;
 int max_sell_item_cap;
 int max_seal_item_val;
 int config_equipment_drop_auto_identify;
 int config_random_monster_rank;
 int config_monster_rank_damage_mode;
 int config_monster_rank_defense_mode;
 int config_monster_rank_exp_bonus_mode;
 int config_monster_rank_drop_bonus_mode;
 int config_enchantment_clickable;
 int config_enchantment_maximum;
 // auto attack
 int autoattack_skillheal;
 int autoattack_item_potion;
 int autoattack_sittorest;
 int autoattack_skill_attack;
 int autoattack_skill_buff;
 int autoattack_item_buff;
 int autoattack_teleport;
 int autoattack_item_pickup;
 int autoattack_mselection;
 int autoattack_item_range_detection;
 int autoattack_mob_detection;
 int autoattack_move;
 int autoattack_move_type;
 int autoattack_interval_timer;
 int autoattack_pickup_delay;
 int autoattack_clean_logout;
 int autoattack_hateffect;
 int autoattack_skill_rate_default;
 int autoattack_reduce_droprate;
 int autoattack_reduce_mode;
 int autoattack_max_moblist;
 int autoattack_max_itemlist;
 int autoattack_steal_reduce_rate;
 int autoattack_max_time_per_mob;
 int autoattack_mob_struck;
 int autoattack_quiver_check;
 int autoattack_max_flee_mob;
 int autoattack_gunslinger_conflip_fix;
 int autoattack_coinflip_coin;