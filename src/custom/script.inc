// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Script Commands
 * Place the body of custom script commands in this file.
 * Format:
 *	BUILDIN_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//BUILDIN_FUNC(example)
//{
//	ShowInfo("buildin_example: It works!\n");
//	script_pushint(st,1);
//	return 0;
//}

BUILDIN_FUNC(specialeffect3)
{
	int type = script_getnum(st, 2);
	enum send_target target = script_hasdata(st, 3) ? (send_target)script_getnum(st, 3) : AREA;

	struct block_list* bl;

	if (!script_rid2bl(4, bl))
		return SCRIPT_CMD_SUCCESS;

	if( type <= EF_NONE || type >= EF_MAX ){
		ShowError( "buildin_specialeffect3: unsupported effect id %d.\n", type );
		return SCRIPT_CMD_FAILURE;
	}

	clif_specialeffect(bl, type, target);

	return SCRIPT_CMD_SUCCESS;
}
