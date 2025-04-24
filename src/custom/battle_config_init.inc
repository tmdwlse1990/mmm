// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config initializations
 * Place the definition of custom battle configuration initializations in this file.
 *
 * Do not forget to add a comma at the end of the line, if you want to add another config
 *
 * Format:
 *	 { "name", &battle_config.<variable name>, <default value>, <minimum value>, <maximum value> },
 **/


 { "mob_mvp_skill_rate", &battle_config.mob_mvp_skill_rate, 1, 0, 1 },
 { "max_sell_item_cap", &battle_config.max_sell_item_cap, 2000, 0, INT_MAX },
 { "max_seal_item_val", &battle_config.max_seal_item_val, 700, 0, INT_MAX },
 { "config_equipment_drop_auto_identify", &battle_config.config_equipment_drop_auto_identify, 1, 0, 1 },
 { "config_random_monster_rank", &battle_config.config_random_monster_rank, 91, 0, 50000 },
 { "config_monster_rank_damage_mode", &battle_config.config_monster_rank_damage_mode, 0, 0, 1 },
 { "config_monster_rank_defense_mode", &battle_config.config_monster_rank_defense_mode, 0, 0, 1 },
 { "config_monster_rank_exp_bonus_mode", &battle_config.config_monster_rank_exp_bonus_mode, 0, 0, 1 },
 { "config_monster_rank_drop_bonus_mode", &battle_config.config_monster_rank_drop_bonus_mode, 0, 0, 1 },
 { "config_enchantment_clickable", &battle_config.config_enchantment_clickable, 1, 0, 1 },
 { "config_enchantment_maximum", &battle_config.config_enchantment_maximum, 1, 0, 4 },