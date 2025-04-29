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


 { "config_maxlv", &battle_config.config_maxlv, 99, 1, MAX_LEVEL },
 { "config_global_mvp_drop", &battle_config.config_global_mvp_drop, 0, 0, INT_MAX },
 { "config_global_mvp_drop_rate", &battle_config.config_global_mvp_drop_rate, 1, 0, INT_MAX },
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
 //autoattack
 { "autoattack_skillheal",				&battle_config.autoattack_skillheal,			1,		0,		1,				},
 { "autoattack_item_potion",				&battle_config.autoattack_item_potion,			1,		0,		1,				},
 { "autoattack_sittorest",				&battle_config.autoattack_sittorest,			1,		0,		1,				},
 { "autoattack_skill_attack",			&battle_config.autoattack_skill_attack,			1,		0,		1,				},
 { "autoattack_skill_buff",				&battle_config.autoattack_skill_buff,			1,		0,		1,				},
 { "autoattack_item_buff",				&battle_config.autoattack_item_buff,			1,		0,		1,				},
 { "autoattack_teleport",				&battle_config.autoattack_teleport,				1,		0,		1,				},
 { "autoattack_item_pickup",				&battle_config.autoattack_item_pickup,			1,		0,		1,				},
 { "autoattack_mselection",				&battle_config.autoattack_mselection,			1,      1,      1,      		},
 { "autoattack_item_range_detection",	&battle_config.autoattack_item_range_detection,	15,		0,		30,				},
 { "autoattack_mob_detection",			&battle_config.autoattack_mob_detection,		15,     1,      30,      		},
 { "autoattack_move",					&battle_config.autoattack_move,					7,      1,      20,      		},
 { "autoattack_move_type",				&battle_config.autoattack_move_type,			0,		0,		1,				},
 { "autoattack_interval_timer",			&battle_config.autoattack_interval_timer,		100,	0,		INT_MAX,		},
 { "autoattack_pickup_delay",			&battle_config.autoattack_pickup_delay,			100,	0,		INT_MAX,		},
 { "autoattack_clean_logout",			&battle_config.autoattack_clean_logout,			0,		0,		1,				},
 { "autoattack_hateffect",				&battle_config.autoattack_hateffect,			0,		0,		INT_MAX,		},
 { "autoattack_skill_rate_default",		&battle_config.autoattack_skill_rate_default,	0,		0,		100,			},
 { "autoattack_reduce_droprate",			&battle_config.autoattack_reduce_droprate,		0,		0,		100,  			},
 { "autoattack_reduce_mode",				&battle_config.autoattack_reduce_mode,			0,		0,		1|2|4|8|16|32,  },
 { "autoattack_max_moblist",				&battle_config.autoattack_max_moblist,			0,		0,		INT_MAX,		},
 { "autoattack_max_itemlist",			&battle_config.autoattack_max_itemlist,			0,		0,		INT_MAX,		},
 { "autoattack_steal_reduce_rate",		&battle_config.autoattack_steal_reduce_rate,	0,		0,		100,			},
 { "autoattack_max_time_per_mob",		&battle_config.autoattack_max_time_per_mob,		0,		0,		INT_MAX,		},
 { "autoattack_mob_struck",				&battle_config.autoattack_mob_struck,			0,		0,		INT_MAX,		},
 { "autoattack_quiver_check",			&battle_config.autoattack_quiver_check,			0,		0,		INT_MAX,		},
 { "autoattack_max_flee_mob",			&battle_config.autoattack_max_flee_mob,			0,		0,		INT_MAX,		},
 { "autoattack_gunslinger_conflip_fix",	&battle_config.autoattack_gunslinger_conflip_fix,	0,		0,		1,			},
 { "autoattack_coinflip_coin",			&battle_config.autoattack_coinflip_coin,		0,		0,		INT_MAX,		},
 