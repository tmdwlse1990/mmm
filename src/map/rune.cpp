#include "rune.hpp"

#include <stdlib.h>
#include <iostream>
#include <set>
#include <tuple>

#include <common/cbasetypes.hpp>
#include <common/database.hpp>
#include <common/db.hpp>
#include <common/ers.hpp>
#include <common/nullpo.hpp>
#include <common/random.hpp>
#include <common/showmsg.hpp>
#include <common/strlib.hpp>

#include "clif.hpp"
#include "itemdb.hpp"
#include "log.hpp"
#include "pc.hpp"

using namespace rathena;

RuneItemDecompositionDatabase rune_item_decomposition_db;
RuneDecompositionDatabase runedecomposition_db;
RuneBookDatabase runebook_db;
RuneDatabase rune_db;

const std::string RuneItemDecompositionDatabase::getDefaultLocation() {
	return std::string(db_path) + "/rune_itemdecomp_db.yml";
}

uint64 RuneItemDecompositionDatabase::parseBodyNode(const ryml::NodeRef &node) {
	std::string item_name;

	if( !this->asString(node, "AegisName", item_name) )
		return false;

	std::shared_ptr<item_data> item = item_db.search_aegisname(item_name.c_str());

	if( item == nullptr ) {
		this->invalidWarning(node["AegisName"], "Item %s does not exist, skipping.\n", item_name.c_str());
		return 0;
	}

	std::shared_ptr<s_rune_item_decomposition> itemdecom = this->find(item->nameid);
	bool exists = itemdecom != nullptr;

	if( !exists ) {
		if( !this->nodesExist(node, {"AegisName","Decomposition" }) )
			return 0;

		itemdecom = std::make_shared<s_rune_item_decomposition>();

		itemdecom->nameid = item->nameid;
	}

	if( this->nodeExists(node, "Decomposition") ) {
		const auto& decompositionNode = node["Decomposition"];

		for( const auto& decit : decompositionNode ) {
			std::string typeName;
			c4::from_chars(decit.key(), &typeName);

			uint16 typeNum;

			if( !this->asUInt16(decompositionNode, typeName, typeNum) )
				return 0;

			itemdecom->decompositionRune[typeName] = typeNum;
		}
	}

	if( !exists )
		this->put(itemdecom->nameid, itemdecom);

	return 1;
}

void RuneItemDecompositionDatabase::loadingFinished() {

	for( const auto& entry : *this ) {
		std::shared_ptr<item_data> item = item_db.find(entry.second->nameid);

		if( item == nullptr ) {
			ShowError("Rune Item Decomposition %d item does not exist, skipping.\n", entry.second->nameid);
			continue;
		}

		item->decompositionRune = entry.second->decompositionRune;
	}

	TypesafeYamlDatabase::loadingFinished();
}

/**
 * Get location of rune decomposition database
 * @author [Shakto]
 **/
const std::string RuneDecompositionDatabase::getDefaultLocation() {
	return std::string(db_path) + "/runedecomposition_db.yml";
}

/**
 * Read rune book YML db
 * @author [Shakto]
 **/
uint64 RuneDecompositionDatabase::parseBodyNode(const ryml::NodeRef &node) {
	uint32 id;

	if( !this->asUInt32(node, "Id", id) )
		return 0;

	std::shared_ptr<s_runedecomposition> runedecomposition = this->find(id);
	bool exists = runedecomposition != nullptr;

	if( !exists ) {
		if( !this->nodesExist(node, {"Materials"}) )
			return 0;

		runedecomposition = std::make_shared<s_runedecomposition>();
		runedecomposition->id = id;
	}

	if( this->nodeExists(node, "Materials") ) {
		for( const ryml::NodeRef& materialNode : node["Materials"] ) {
			std::string item_name;

			if( !this->asString(materialNode, "Material", item_name) )
				return false;

			std::shared_ptr<item_data> item = item_db.search_aegisname(item_name.c_str());

			if( item == nullptr ) {
				this->invalidWarning(materialNode["Material"], "Rune Decomposition %d item %s does not exist, skipping.\n", runedecomposition->id, item_name.c_str());
				continue;
			}

			t_itemid nameid = item->nameid;

			std::shared_ptr<s_runedecomposition_material> rune_mats;

			auto isRunedecompomat = runedecomposition->materials.find(nameid);
			bool material_exists = isRunedecompomat != runedecomposition->materials.end();

			if( !material_exists ) {
				rune_mats = std::make_shared<s_runedecomposition_material>();

				rune_mats->nameid = nameid;
			}

			if( this->nodeExists(materialNode, "Min") ) {
				uint16 min;

				if( !this->asUInt16(materialNode, "Min", min) )
					return 0;

				if( min > MAX_AMOUNT ) {
					this->invalidWarning(materialNode["Min"], "Amount %hu is too high, capping to MAX_AMOUNT...\n", min);
					min = MAX_AMOUNT;
				}

				rune_mats->min = min;
			} else {
				rune_mats->min = 0;
			}

			if( this->nodeExists(materialNode, "Max") ) {
				uint16 max;

				if( !this->asUInt16(materialNode, "Max", max) )
					return 0;

				if( max < rune_mats->min ) {
					this->invalidWarning(materialNode["Max"], "max %hu is lower than amountmin, capping to min...\n", max);
					max = rune_mats->min;
				}

				if( max > MAX_AMOUNT ) {
					this->invalidWarning(materialNode["Max"], "max %hu is too high, capping to MAX_AMOUNT...\n", max);
					max = MAX_AMOUNT;
				}

				rune_mats->max = max;
			} else {
				rune_mats->max = rune_mats->min;
			}

			if( this->nodeExists( materialNode,"Chance") ) {
				uint32 chance;

				if( !this->asUInt32Rate(materialNode, "Chance", chance, 100000) ) {
					return 0;
				}

				rune_mats->chance = chance;
			} else {
				rune_mats->chance = 100000;
			}

			if( !material_exists )
				runedecomposition->materials[nameid] = *rune_mats;
		}
	}

	if( !exists )
		this->put(runedecomposition->id, runedecomposition);

	return 1;
}

/**
 * Get location of rune book database
 * @author [Shakto]
 **/
const std::string RuneBookDatabase::getDefaultLocation() {
	return std::string(db_path) + "/runebook_db.yml";
}

/**
 * Read rune book YML db
 * @author [Shakto]
 **/
uint64 RuneBookDatabase::parseBodyNode(const ryml::NodeRef &node) {
	uint32 id;

	if( !this->asUInt32(node, "Id", id) )
		return 0;

	std::shared_ptr<s_runebook> runebook = this->find(id);
	bool exists = runebook != nullptr;

	if( !exists ) {
		if( !this->nodesExist(node, {"Name", "Materials"}) )
			return 0;

		runebook = std::make_shared<s_runebook>();

		runebook->id = id;
	}

	if( this->nodeExists(node, "Name") ) {
		std::string name;

		if( !this->asString(node, "Name", name) )
			return 0;

		runebook->name = name;
	}

	uint32 temp_id = runebook_db.search_name(runebook->name.c_str());

	if( temp_id )
		this->nameToBookId.erase(runebook->name);
	else
		this->nameToBookId[runebook->name] = id;

	if( this->nodeExists(node, "Materials") ) {
		for( const ryml::NodeRef& materialNode : node["Materials"] ) {
			std::string item_name;

			if( !this->asString(materialNode, "Material", item_name) )
				return false;

			std::shared_ptr<item_data> item = item_db.search_aegisname(item_name.c_str());

			if( item == nullptr ) {
				this->invalidWarning(materialNode["Material"], "Rune Book %d item %s does not exist, skipping.\n", runebook->id, item_name.c_str());
				continue;
			}

			if( this->nodeExists(materialNode, "Amount") ) {
				uint16 amount = 0;

				if( !this->asUInt16(materialNode, "Amount", amount) )
					return 0;

				if( amount > MAX_AMOUNT ) {
					this->invalidWarning(materialNode["Amount"], "Amount %hu is too high, capping to MAX_AMOUNT...\n", amount);
					amount = MAX_AMOUNT;
				}

				runebook->materials[item->nameid] = amount;
			}else{
				runebook->materials[item->nameid] = 1;
			}
		}
	}

	if( !exists )
		this->put(runebook->id, runebook);

	return 1;
}

uint32 RuneBookDatabase::search_name(const char* name) {
	uint32 *bookid = util::umap_find(this->nameToBookId, std::string(name));

	if( bookid == nullptr )
		return 0;

	return *bookid;
}

/**
 * Get location of rune book database
 * @author [Shakto]
 **/
const std::string RuneDatabase::getDefaultLocation() {
	return std::string(db_path) + "/rune_db.yml";
}

/**
 * Read rune book YML db
 * @author [Shakto]
 **/
uint64 RuneDatabase::parseBodyNode(const ryml::NodeRef &node) {
	uint16 id;

	if( !this->asUInt16( node, "Id", id ) )
		return 0;

	std::shared_ptr<s_rune> rune = this->find(id);
	bool exists = rune != nullptr;

	if( !exists ) {
		if( !this->nodesExist(node, {"Name", "Set"}) )
			return 0;

		rune = std::make_shared<s_rune>();

		rune->id = id;
	}

	if( this->nodeExists(node, "Name") ) {
		std::string name;

		if( !this->asString(node, "Name", name) )
			return 0;

		rune->name = name;
	}

	if( this->nodeExists(node, "Set") ) {
		for( const ryml::NodeRef& setNode : node["Set"] ) {
			uint32 setid;

			if( !this->asUInt32(setNode, "Id", setid) )
				return 0;

			std::shared_ptr<s_rune_set> set = util::umap_find(rune->sets, setid);

			bool set_exists = set != nullptr;

			if( !set_exists ) {

				if( !this->nodesExist(setNode, {"Name", "Books"}) )
					return 0;

				set = std::make_shared<s_rune_set>();

				set->id = setid;
			}

			if( this->nodeExists(setNode, "Name") ) {
				std::string setname;

				if( !this->asString(setNode, "Name", setname) )
					return 0;

				set->name = setname;
			}

			if( this->nodeExists(setNode, "Books") ) {
				for( const ryml::NodeRef& bookNode : setNode["Books"] ) {
					uint16 slot;

					if( !this->asUInt16(bookNode, "Slot", slot) )
						return 0;

					if( slot > MAX_RUNESLOT ) {
						this->invalidWarning(bookNode["Slot"], "Slot %hu is too high...\n", slot);
						return 0;
					}

					uint32* slot_bookid = util::umap_find(set->slots, slot);
					bool slot_exists = slot_bookid != nullptr;

					if( slot_exists )
						set->slots.erase(slot);

					if( this->nodeExists(bookNode, "Name") ) {
						std::string bookname;

						if( !this->asString(bookNode, "Name", bookname) )
							return 0;

						uint32 bookid = runebook_db.search_name(bookname.c_str());

						if( !bookid ) {
							this->invalidWarning(bookNode["Name"], "Rune Book %d set %d book %s does not exist, skipping.\n", rune->id, set->id, bookname.c_str());
							return 0;
						}

						set->slots[slot] = bookid;
					}
				}
			}

			if( this->nodeExists(setNode, "Reward") ) {
				for( const ryml::NodeRef& rewardNode : setNode["Reward"] ) {
					uint16 slot;

					if( !this->asUInt16(rewardNode, "Slot", slot) )
						return 0;

					if( slot <= 0 || slot > MAX_REWARDSLOT ) {
						this->invalidWarning(rewardNode["Slot"], "Slot %hu, need to be between 0 and %d\n", slot, MAX_REWARDSLOT);
						return 0;
					}

					std::string item_name;

					if( !this->asString(rewardNode, "Name", item_name) )
						return false;

					std::shared_ptr<item_data> item = item_db.search_aegisname(item_name.c_str());

					if( item == nullptr ) {
						this->invalidWarning(rewardNode["Activation"], "Rune Reward of set %d item %s does not exist, skipping.\n", set->id, item_name.c_str());
						continue;
					}

					t_itemid material_id = item->nameid;

					set->rewards[slot] = item->nameid;
				}
			}

			if( this->nodeExists(setNode, "Activation") ) {
				for( const ryml::NodeRef& activationNode : setNode["Activation"] ) {
					std::string item_name;

					if( !this->asString(activationNode, "Material", item_name) )
						return false;

					std::shared_ptr<item_data> item = item_db.search_aegisname(item_name.c_str());

					if( item == nullptr ) {
						this->invalidWarning(activationNode["Activation"], "Rune Book %d item %s does not exist, skipping.\n", set->id, item_name.c_str());
						continue;
					}

					t_itemid material_id = item->nameid;
					bool material_exists = util::umap_find(set->activation_materials, material_id) != nullptr;
					uint16 amount;

					if( this->nodeExists(activationNode, "Amount") ) {
						if( !this->asUInt16(activationNode, "Amount", amount) )
							return 0;

						if( amount > MAX_AMOUNT ) {
							this->invalidWarning(activationNode["Amount"], "Amount %hu is too high, capping to MAX_AMOUNT...\n", amount);

							amount = MAX_AMOUNT;
						}
					} else {
						if( !material_exists )
							amount = 1;
					}

					if( amount > 0 )
						set->activation_materials[material_id] = amount;
					else
						set->activation_materials.erase(material_id);
				}
			}

			if( this->nodeExists(setNode, "Scripts") ) {
				for( const ryml::NodeRef& scriptNode : setNode["Scripts"] ) {
					uint16 amount;

					if( !this->asUInt16(scriptNode, "Amount", amount) )
						return 0;

					std::shared_ptr<s_rune_script> script_data = util::umap_find(set->scripts, amount);
					bool script_exists = script_data != nullptr;

					if( !script_exists ) {
						script_data = std::make_shared<s_rune_script>();

						script_data->amount = amount;
					} else
						return 0;

					if( this->nodeExists(scriptNode, "Script") ) {
						std::string script;

						if( !this->asString(scriptNode, "Script", script) )
							return 0;

						if( set->scripts[amount] && set->scripts[amount]->script ) {
							aFree(set->scripts[amount]->script);

							set->scripts[amount]->script = nullptr;
						}

						script_data->script = parse_script(script.c_str(), this->getCurrentFile().c_str(), this->getLineNumber(scriptNode["Script"]), SCRIPT_IGNORE_EXTERNAL_BRACKETS);
					}

					if( !script_exists )
						set->scripts[amount] = script_data;
				}
			}

			if( this->nodeExists(setNode, "Upgrades") ) {
				for( const ryml::NodeRef& upgradeNode : setNode["Upgrades"] ) {
					uint16 grade;

					if( !this->asUInt16(upgradeNode, "Grade", grade) )
						return 0;

					std::shared_ptr<s_rune_upgrade> upgrade = util::umap_find(set->upgrades, grade);
					bool upgrade_exists = upgrade != nullptr;

					if( !upgrade_exists ) {
						upgrade = std::make_shared<s_rune_upgrade>();

						upgrade->grade = grade;
					} else
						return 0;

					if( this->nodeExists(upgradeNode, "Chance") ) {
						uint32 chance;

						if( !this->asUInt32Rate(upgradeNode, "Chance", chance, 100000) ) {
							return 0;
						}

						upgrade->chance = chance;
					}

					if( this->nodeExists(upgradeNode, "ChancePerFail") ) {
						uint32 chanceperfail;

						if( !this->asUInt32Rate(upgradeNode, "ChancePerFail", chanceperfail, 100000) ) {
							return 0;
						}

						upgrade->chanceperfail = chanceperfail;
					}

					if( this->nodeExists(upgradeNode, "Materials") ) {
						for( const ryml::NodeRef& setmaterialNode : upgradeNode["Materials"] ) {
							std::string item_name;

							if( !this->asString(setmaterialNode, "Material", item_name) )
								return false;

							std::shared_ptr<item_data> item = item_db.search_aegisname(item_name.c_str());

							if( item == nullptr ) {
								this->invalidWarning(setmaterialNode["Materials"], "Rune Book %d grade %d item %s does not exist, skipping.\n", set->id, grade, item_name.c_str());
								continue;
							}

							t_itemid material_id = item->nameid;
							bool material_exists = util::umap_find(upgrade->materials, material_id) != nullptr;
							uint16 amount = 0;

							if( this->nodeExists(setmaterialNode, "Amount") ) {
								if( !this->asUInt16(setmaterialNode, "Amount", amount) )
									return 0;

								if( amount > MAX_AMOUNT ) {
									this->invalidWarning(setmaterialNode["Amount"], "Amount %hu is too high, capping to MAX_AMOUNT...\n", amount);

									amount = MAX_AMOUNT;
								}
							} else {
								if( !material_exists )
									amount = 1;
							}

							if( amount > 0 )
								upgrade->materials[material_id] = amount;
							else
								upgrade->materials.erase(material_id);
						}
					}

					if( !upgrade_exists )
						set->upgrades[grade] = upgrade;
				}
			}

			if( !set_exists ) {
				rune->sets[setid] = set;
			}

		}
	} else
		return 0;

	if( !exists )
		this->put(rune->id, rune);

	return 1;
}

void intif_rune_save(map_session_data* sd) {
	if( sd->runeSets.size() ) {
		for( const auto& set_data : sd->runeSets ) {
			if( SQL_ERROR == Sql_Query(mmysql_handle, "INSERT IGNORE INTO `runes` (`char_id`, `rune_id`, `set_id`, `selected`, `upgrade`, `failcount`, `reward`) VALUES (%u, %u, %u, %u, %u, %u, %u) ON DUPLICATE KEY UPDATE `selected` = %u, `upgrade` = %u, `failcount` = %u, `reward` = %u", sd->status.char_id, set_data.tagId, set_data.setId, set_data.selected, set_data.upgrade, set_data.failcount, set_data.reward, set_data.selected, set_data.upgrade, set_data.failcount, set_data.reward) ) {
				Sql_ShowDebug(mmysql_handle);
			}
		}
	}

	if( sd->runeBooks.size() ) {
		for( const auto& book_data : sd->runeBooks ) {
			if( SQL_ERROR == Sql_Query(mmysql_handle, "INSERT IGNORE INTO `runes_book` (`char_id`, `rune_id`, `book_id`) VALUES (%u, %u, %u) ", sd->status.char_id, book_data.tagId, book_data.bookId) ) {
				Sql_ShowDebug(mmysql_handle);
			}
		}
	}
}

void intif_rune_load(map_session_data* sd) {
	sd->runeSets.clear();
	sd->runeBooks.clear();

	if( Sql_Query(mmysql_handle, "SELECT `rune_id`, `set_id`, `selected`, `upgrade`, `failcount`, `reward` FROM `runes` WHERE `char_id` = %d", sd->status.char_id) != SQL_SUCCESS) {
		Sql_ShowDebug(mmysql_handle);
	}

	while(SQL_SUCCESS == Sql_NextRow(mmysql_handle)) {
		s_runeset_data set_data = {};
		char* data;

		Sql_GetData(mmysql_handle, 0, &data, nullptr); set_data.tagId = atoi(data);
		Sql_GetData(mmysql_handle, 1, &data, nullptr); set_data.setId = strtoul(data, nullptr, 10);
		Sql_GetData(mmysql_handle, 2, &data, nullptr); set_data.selected = atoi(data);
		Sql_GetData(mmysql_handle, 2, &data, nullptr);

		if( data ) {
			set_data.selected = atoi(data);
		} else {
			set_data.selected = 0;
		}

		Sql_GetData(mmysql_handle, 3, &data, nullptr); set_data.upgrade = atoi(data);
		Sql_GetData(mmysql_handle, 4, &data, nullptr); set_data.failcount = atoi(data);
        Sql_GetData(mmysql_handle, 5, &data, nullptr); set_data.reward = atoi(data);

		sd->runeSets.push_back(set_data);
	}
	Sql_FreeResult(mmysql_handle);

	if( Sql_Query(mmysql_handle, "SELECT `rune_id`, `book_id` FROM `runes_book` WHERE `char_id` = %d", sd->status.char_id) != SQL_SUCCESS) {
		Sql_ShowDebug(mmysql_handle);
	}

	while(SQL_SUCCESS == Sql_NextRow(mmysql_handle)) {
		s_runebook_data book = {};
		char* data;

		Sql_GetData(mmysql_handle, 0, &data, nullptr); book.tagId = atoi(data);
		Sql_GetData(mmysql_handle, 1, &data, nullptr); book.bookId = strtoul(data, nullptr, 10);

		sd->runeBooks.push_back(book);
	}
	Sql_FreeResult(mmysql_handle);

	sd->runeActive = {};
	sd->runeActive.loaded = false;
	sd->runeActive.tagID = 0;
	sd->runeActive.upgrade = 0;
	sd->runeActive.runesetid = 0;
	sd->runeActive.bookNumber = 0;
	sd->runeActive.failcount = 0;

	if( sd->runeSets.size() ) {
		for( const auto& set_data : sd->runeSets ) {
			if( set_data.selected ) {
				sd->runeActive.tagID = set_data.tagId;
				sd->runeActive.runesetid = set_data.setId;
				sd->runeActive.upgrade = set_data.upgrade;
				sd->runeActive.failcount = set_data.failcount;
				break;
			}
		}
	}

	if( sd->runeActive.tagID )
		clif_enable_rune(sd);
}

int32 rune_bookactivate(map_session_data* sd, uint16 tagID, uint32 runebookid) {
	if( !sd )
		return ZC_RUNESET_TABLET_INVALID2;

	uint32 runesetid = 0;

	std::shared_ptr<s_runebook> runebook = runebook_db.find(runebookid);

	if( runebook == nullptr )
		return ZC_RUNESET_TABLET_INVALID2;

	std::shared_ptr<s_rune> rune = rune_db.find(tagID);

	if( rune == nullptr )
		return ZC_RUNESET_TABLET_INVALID2;

	bool isBookinSet = false;

	if( rune->sets.size() ) {
		for( const auto& set_pair : rune->sets ) {
			for( const auto& slot_pair : set_pair.second->slots ) {
				if( slot_pair.second == runebookid ) {
					runesetid = set_pair.first;
					isBookinSet = true;
					break;
				}
			}

			if( isBookinSet )
				break;
		}
	}

	if( !isBookinSet )
		return ZC_RUNESET_TABLET_INVALID2;

	auto it_setrune_data = std::find_if(sd->runeBooks.begin(), sd->runeBooks.end(), [tagID, runebookid](const s_runebook_data& runeBook) { return runeBook.tagId == tagID && runeBook.bookId == runebookid; });

	if( it_setrune_data != sd->runeBooks.end() ) {
		return ZC_RUNEBOOK_ALRDYACTIVATED;
	}

	std::unordered_map<t_itemid, uint16> materials;

	for( const auto& entry : runebook->materials ) {
		int16 idx = pc_search_inventory(sd, entry.first);

		if( idx < 0 ) {
			return ZC_RUNEBOOK_NOITEM;
		}

		if( sd->inventory.u.items_inventory[idx].amount < entry.second ) {
			return ZC_RUNEBOOK_NOITEM;
		}

		materials[idx] = entry.second;
	}

	for( const auto& entry : materials ) {
		if( pc_delitem(sd, entry.first, entry.second, 0, 0, LOG_TYPE_ENCHANT)  != 0 ) {
			return ZC_RUNEBOOK_NOITEM;
		}
	}

	s_runebook_data book = {};
	book.tagId = tagID;
	book.bookId = runebookid;

	sd->runeBooks.push_back(book);

	if( sd->runeActive.runesetid ) {
		rune_count_bookactivated(sd, tagID, runesetid);
		status_calc_pc(sd, SCO_FORCE);
	}

	return ZC_RUNEBOOK_SUCCESS;
}

int32 rune_setactivate(map_session_data* sd, uint16 tagID, uint32 runesetid) {
	if( !sd )
		return ZC_RUNESET_TABLET_INVALID;

	bool isRunetag = false;
	auto it_setrune_data = std::find_if(sd->runeSets.begin(), sd->runeSets.end(), [tagID, runesetid](const s_runeset_data& runeSet) { return runeSet.tagId == tagID && runeSet.setId == runesetid; });

	if( it_setrune_data != sd->runeSets.end() ) {
		return ZC_RUNESET_ALRDYACTIVATED;
	}

	std::shared_ptr<s_rune> rune = rune_db.find(tagID);

	if( rune == nullptr )
		return ZC_RUNESET_TABLET_INVALID;

	bool isSet = false;

	for( const auto& set_pair : rune->sets ) {
		if( set_pair.first == runesetid ) {
			const auto& set_data = set_pair.second;

			std::unordered_map<t_itemid, uint16> materials;

			for( const auto& entry : set_data->activation_materials ) {
				int16 idx = pc_search_inventory(sd, entry.first);

				if( idx < 0 ) {
					return ZC_RUNEBOOK_NOITEM;
				}

				if( sd->inventory.u.items_inventory[idx].amount < entry.second ) {
					return ZC_RUNEBOOK_NOITEM;
				}

				materials[idx] = entry.second;
			}

			for( const auto& entry : materials ) {
				if( pc_delitem(sd, entry.first, entry.second, 0, 0, LOG_TYPE_ENCHANT)  != 0 ) {
					return ZC_RUNEBOOK_NOITEM;
				}
			}

			isSet = true;
			break;
		}
	}

	if( !isSet )
		return ZC_RUNESET_TABLET_INVALID;

	s_runeset_data set_data = {};
	set_data.tagId = tagID;
	set_data.setId = runesetid;
	set_data.selected = 0;
	set_data.upgrade = 0;
	set_data.failcount = 0;
    set_data.reward = 0;
	sd->runeSets.push_back(set_data);

	return ZC_RUNESET_SUCCESS;
}

std::tuple<e_runereward_result, uint8> rune_askreward(map_session_data* sd, uint16 tagID, uint32 runesetid, uint8 reward) {
	uint8 bookNumber = 0;
	enum e_additem_result flag;
	t_itemid item_id;
    size_t max_reward = 0;

	auto it_setrune_data = std::find_if(sd->runeSets.begin(), sd->runeSets.end(), [tagID, runesetid](const s_runeset_data& runeSet) { return runeSet.tagId == tagID && runeSet.setId == runesetid; });

	if( it_setrune_data == sd->runeSets.end() )
		return std::make_tuple(ZC_RUNEREWARD_FAIL, 0);

	std::shared_ptr<s_rune> rune_data = rune_db.find(tagID);

	if( rune_data == nullptr )
		return std::make_tuple(ZC_RUNEREWARD_INVALID, 0);

	std::shared_ptr<s_rune_set> runeset_data = util::umap_find(rune_data->sets, runesetid);

	if( runeset_data == nullptr )
		return std::make_tuple(ZC_RUNEREWARD_INVALID, 0);

	auto reward_it = runeset_data->rewards.find(reward);

	if( reward_it == runeset_data->rewards.end() )
		return std::make_tuple(ZC_RUNEREWARD_INFONOTMATCH, 0);
	else {
        item_id = reward_it->second;
    }

    max_reward = runeset_data->rewards.size();

	if( it_setrune_data->reward >= reward )
		return std::make_tuple(ZC_RUNEREWARD_FAIL, 0);

	for( const auto& slot_pair : runeset_data->slots ) {
		uint32 bookId = slot_pair.second;

		for( const auto& book : sd->runeBooks ) {
			if( book.tagId == tagID && book.bookId == bookId ) {
				bookNumber++;
			}
		}
	}

	if( bookNumber < reward && reward != MAX_REWARDSLOT )
		return std::make_tuple(ZC_RUNEREWARD_FAIL, 0);

	if( reward == MAX_REWARDSLOT && bookNumber < runeset_data->slots.size() )
		return std::make_tuple(ZC_RUNEREWARD_FAIL, 0);

	struct item item_tmp = {};

	item_tmp.nameid = item_id;
	item_tmp.identify = 1;
	item_tmp.bound = 0;

	if( flag = pc_additem(sd, &item_tmp, 1, LOG_TYPE_COMMAND) ) {
		clif_additem(sd, 0, 0, flag);
	}

	switch(flag) {
		case ADDITEM_SUCCESS:
			it_setrune_data->reward = reward;
			return std::make_tuple(ZC_RUNEREWARD_SUCCESS, reward);
			break;

		case ADDITEM_INVALID:
			return std::make_tuple(ZC_RUNEREWARD_FAIL, 0);
			break;

		case ADDITEM_OVERWEIGHT:
			return std::make_tuple(ZC_RUNEREWARD_WEIGH, 0);
			break;

		case ADDITEM_ITEM:
			return std::make_tuple(ZC_RUNEREWARD_MAXITEMS, 0);
			break;

		case ADDITEM_OVERITEM:
			return std::make_tuple(ZC_RUNEREWARD_INVENTORYSPACE, 0);
			break;

		default:
			return std::make_tuple(ZC_RUNEREWARD_FAIL, 0);
			break;
	}

	return std::make_tuple(ZC_RUNEREWARD_FAIL, 0);
}

std::tuple<uint8, uint16, uint16> rune_setupgrade(map_session_data* sd, uint16 tagID, uint32 runesetid) {
	if( !sd )
		return std::make_tuple(ZC_RUNESET_TABLET_INVALID, 0, 0);

	uint16 upgrade = 0;
	uint16 failcount = 0;

	auto it_setrune_data = std::find_if(sd->runeSets.begin(), sd->runeSets.end(), [tagID, runesetid](const s_runeset_data& runeSet) { return runeSet.tagId == tagID && runeSet.setId == runesetid; });

	if( it_setrune_data != sd->runeSets.end() ) {
		upgrade = it_setrune_data->upgrade;
		failcount = it_setrune_data->failcount;
	} else
		return std::make_tuple(ZC_RUNESET_TABLET_INVALID3, 0, 0);

	std::shared_ptr<s_rune> rune_data = rune_db.find(tagID);

	if( rune_data == nullptr )
		return std::make_tuple(ZC_RUNESET_TABLET_INVALID, 0, 0);

	std::shared_ptr<s_rune_set> runeset_data = util::umap_find(rune_data->sets, runesetid);

	if( runeset_data == nullptr )
		return std::make_tuple(ZC_RUNESET_TABLET_INVALID, 0, 0);

	std::shared_ptr<s_rune_upgrade> runeupgrade_data = util::umap_find(runeset_data->upgrades, upgrade);

	if( runeupgrade_data == nullptr )
		return std::make_tuple(ZC_RUNESET_TABLET_INVALID, 0, 0);

	std::unordered_map<t_itemid, uint16> materials;

	for( const auto& entry : runeupgrade_data->materials ) {
		int16 idx = pc_search_inventory(sd, entry.first);

		if( idx < 0 ) {
			return std::make_tuple(ZC_RUNESET_NOITEM, upgrade, failcount);
		}

		if( sd->inventory.u.items_inventory[idx].amount < entry.second ) {
			return std::make_tuple(ZC_RUNESET_NOITEM, upgrade, failcount);
		}

		materials[idx] = entry.second;
	}

	for( const auto& entry : materials ) {
		if( pc_delitem( sd, entry.first, entry.second, 0, 0, LOG_TYPE_ENCHANT )  != 0 ) {
			return std::make_tuple(ZC_RUNESET_NOITEM, upgrade, failcount);
		}
	}

	uint32 chance = runeupgrade_data->chance + (runeupgrade_data->chanceperfail * failcount);

	if( chance == 0 ) {
		return std::make_tuple(ZC_RUNESET_TABLET_INVALID, 0, 0);
	}

	if( chance < 100000 && rnd_value( 0, 100000 ) > chance )
		failcount++;
	else {
		upgrade++;
		failcount = 0;
	}

	it_setrune_data->upgrade = upgrade;
	it_setrune_data->failcount = failcount;

	if( sd->runeActive.runesetid && sd->runeActive.tagID == tagID && sd->runeActive.runesetid == runesetid ) {
			sd->runeActive.upgrade = static_cast<uint8>(upgrade);
			status_calc_pc(sd, SCO_FORCE);
	}

	return std::make_tuple(ZC_RUNESET_SUCCESS, upgrade, failcount);
}

bool rune_changestate(map_session_data* sd, uint16 tagID, uint32 runesetid) {
	if( !sd )
		return false;

	for( auto& set_data : sd->runeSets ) {
		if( set_data.tagId == tagID && runesetid && set_data.setId == runesetid ) {
			sd->runeActive.tagID = set_data.tagId;
			sd->runeActive.runesetid = set_data.setId;
			sd->runeActive.upgrade = set_data.upgrade;
			rune_count_bookactivated(sd, tagID, runesetid);
			set_data.selected = true;

			status_calc_pc(sd, SCO_FORCE);
			return true;
		}

		if( set_data.tagId == tagID && !runesetid && set_data.selected ) {
			sd->runeActive.tagID = 0;
			sd->runeActive.runesetid = 0;
			sd->runeActive.upgrade = 0;
			sd->runeActive.bookNumber = 0;
			set_data.selected = false;

			status_calc_pc(sd, SCO_FORCE);
			return false;
		}
	}

	return false;
}

void rune_count_bookactivated(map_session_data* sd, uint16 tagID, uint32 runesetid) {
	nullpo_retv(sd);

	sd->runeActive.bookNumber = 0;

	std::shared_ptr<s_rune> rune_data = rune_db.find(tagID);

	if( rune_data == nullptr )
		return;

	std::shared_ptr<s_rune_set> runeset_data = util::umap_find(rune_data->sets, runesetid);

	if( runeset_data == nullptr )
		return;

    for( const auto& slot_pair : runeset_data->slots ) {
        uint32 bookId = slot_pair.second;

        bool bookFound = false;

        for( const auto& book : sd->runeBooks ) {
            if( book.tagId == tagID && book.bookId == bookId ) {
                sd->runeActive.bookNumber++;
            }
        }
    }
}

void rune_active_bonus(map_session_data *sd) {
	nullpo_retv(sd);

	if( !sd->runeActive.tagID )
		return;

	std::shared_ptr<s_rune> rune_data = rune_db.find(sd->runeActive.tagID);

	if( rune_data == nullptr )
		return;

	std::shared_ptr<s_rune_set> runeset_data = util::umap_find(rune_data->sets, sd->runeActive.runesetid);

	if( runeset_data == nullptr )
		return;

    for( const auto& script_pair : runeset_data->scripts ) {
         const std::shared_ptr<s_rune_script> script_data = script_pair.second;

		if( sd->runeActive.bookNumber >= script_data->amount ) {
			run_script(script_data->script, 0, sd->id, 0);
		}
    }

	if( sd->runeActive.tagID && sd->runeActive.loaded ) {
		clif_enablerefresh_rune2(sd, 0, 0);
		clif_enablerefresh_rune2(sd, sd->runeActive.tagID, sd->runeActive.runesetid);
	}
}

void rune_db_reload() {
	do_final_rune();
	do_init_rune();
}

void do_init_rune() {
	rune_item_decomposition_db.load();
	runedecomposition_db.load();
	runebook_db.load();
	rune_db.load();
}

void do_final_rune() {
	rune_item_decomposition_db.clear();
	runedecomposition_db.clear();
	runebook_db.clear();
	rune_db.clear();
}
