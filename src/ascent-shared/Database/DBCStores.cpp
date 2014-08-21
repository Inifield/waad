/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentcommunity.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "DBCStores.h"
#include "DataStore.h"
#include "NGLog.h"

SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
SERVER_DECL DBCStorage<CharTitlesEntry> dbcCharTitlesEntry;
SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
SERVER_DECL DBCStorage<LockEntry> dbcLock;
SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
SERVER_DECL DBCStorage<EmoteTextEntry> dbcEmoteEntry;
SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
SERVER_DECL DBCStorage<AreaTable> dbcArea;
SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
SERVER_DECL DBCStorage<CreatureDisplayInfo> dbcCreatureDisplayInfo;
SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
SERVER_DECL DBCStorage<MapEntry> dbcMap;
SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices;
SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyle;
SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
SERVER_DECL DBCStorage<SpellShapeshiftForm> dbcSpellShapeshiftForm;
SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProperties;
SERVER_DECL DBCStorage<RuneCostEntry> dbcSpellRuneCost;
SERVER_DECL DBCStorage<AchievementEntry> dbcAchievement;
SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchivementCriteria; 
SERVER_DECL DBCStorage<AchievementCategoryEntry> dbcAchievementCategory;
SERVER_DECL DBCStorage<VehicleEntry> dbcVehicle;        
SERVER_DECL DBCStorage<VehicleSeatEntry> dbcVehicleSeat;
SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;
SERVER_DECL DBCStorage<WorldSafeLocsStoreEntry> dbcWorldSafeLocsStore;
SERVER_DECL DBCStorage<CurrencyTypesEntry> dbcCurrencyTypes;
SERVER_DECL DBCStorage<QuestXPLevel> dbcQuestXPLevel;
SERVER_DECL DBCStorage<QuestFactionRewardEntry> dbcQuestFactionReward;
SERVER_DECL DBCStorage<DestructibleModelDataEntry> dbcDestructibleModelDataEntry;
// Arcemu
SERVER_DECL DBCStorage<AreaTableEntry> dbcWMOAreaTable;

                               //         11111111112222222222333333333344444444445555555555666         
                               //12345678901234567890123456789012345678901234567890123456789012
const char* AreaTriggerFormat = "uuffffffff"; // 332.11403 - Idem 335.12340 Fr
const char* AreaGroupFormat   = "uuuuuuuu";   // 332.11403 - Idem 335.12340 Fr
//const char* areatableFormat = "uuuuuxxxuxusxxxxxxxxxxxxxxxxuxxxxxxx"; // 309 - 322 - 332.11403
const char* areatableFormat   = "uuuuuxxxuxuxxsxxxxxxxxxxxxxxuxxxxxxx"; // 335.12340 Fr
const char* auctionhousedbcFormat = "uuuuxxxxxxxxxxxxxxxxx"; // 332.11403

//const char* CharTitlesEntryfmt = "uxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxu"; // 322 - 332.11403
const char* CharTitlesEntryfmt   = "uxxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxu"; // 335.12340 Fr

const char* currencyTypesFormat = "xuxu";

//const char* ItemSetFormat = "usxxxxxxxxxxxxxxxuuuuuuuuuuuxxxxxxxuuuuuuuuuuuuuuuuuu"; // 322 - 322.11403
const char* ItemSetFormat   = "uxxsxxxxxxxxxxxxxuuuuuuuuuuuxxxxxxxuuuuuuuuuuuuuuuuuu"; // 335.12340 Fr
const char* itemextendedcostFormat = "uuuxuuuuuuuuuuux";  // 322 - 332.11403 - 335.12340 Fr

const char* LockFormat = "uuuuuuxxxuuuuuxxxuuuuuxxxxxxxxxxx"; // 335.12340 
const char* EmoteTextEntryFormat = "uxuuuuxuxuxuuxxuxxx";     // 332.11403 - idem 335.12340 Fr
const char* SkillLineSpellFormat = "uuuuuuuuuuuuuu";          // 332.11403 - idem 335.12340 Fr
//const char* SkillLineEntryFormat = "uuusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 332.11403
const char* SkillLineEntryFormat   = "uuuxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 335.12340

//const char* EnchantEntryFormat = "uxuuuuuuuuuuuusxxxxxxxxxxxxxxxxuuuuuuu"; // 332
const char* EnchantEntryFormat   = "uxuuuuuuuuuuuuxxsxxxxxxxxxxxxxxuuuuuuu"; // 335.12340 Fr

const char* GemPropertyEntryFormat   = "uuuuu"; // 335.12340
const char* GlyphPropertyEntryFormat = "uuuu";  // 335.12340

// const char* talententryFormat = "uuuuuuuuuxxxxuxxuxxxxxx"; // 309
const char* talententryFormat    = "uuuuuuuuuxxxxuxxuxxuxxx"; // 332.11403 - idem 335.12340 Fr
// const char* talenttabentryFormat = "uxxxxxxxxxxxxxxxxxxxuuxx"; // 309
const char* talenttabentryFormat    = "uxxxxxxxxxxxxxxxxxxuuxux"; // 332.11403 - idem 335.12340 Fr

const char* spellcasttimeFormat = "uuxx";                                     // 332 - 332.11403 - idem 335.12340 Fr
const char* spellradiusFormat   = "ufxf";                                     // 322 - 332.11403 - idem 335.12340 Fr
const char* spellrangeFormat    = "uffffxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 322 - 332.11403 - idem 335.12340 Fr
const char* spelldurationFormat = "uuuu";                                     // 322 - 332.11403 - idem 335.12340 Fr
const char* randompropsFormat   = "uxuuuxxxxxxxxxxxxxxxxxxx";                 // 322 - 332.11403 - idem 335.12340 Fr
const char* factiontemplatedbcFormat = "uuuuuuuuuuuuuu";                      // 335.12340
//const char* factiondbcFormat = "uiuuuuxxxxiiiixxxxuxxxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 332.11403
const char* factiondbcFormat   = "uiuuuuxxxxiiiixxxxuxxxxxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 335.12340

//const char* dbctaxinodeFormat   = "uufffsxxxxxxxxxxxxxxxxuu"; // 322 - 332.11403
const char* dbctaxinodeFormat     = "uufffxxsxxxxxxxxxxxxxxuu"; // 335.12340 Fr

const char* dbctaxipathFormat     = "uuuu";        // 335.12340
const char* dbctaxipathnodeFormat = "uuuufffuuuu"; // 335.12340 + Gestion des events sur scripts
const char* creaturedisplayFormat = "uxxxfxxxxxxxxxxx"; // 322 - 332.11403 - idem 335.12340 Fr
const char* creaturespelldataFormat = "uuuuuuuuu"; // 309 - 322 - 332.11403 - idem 335.12340 Fr

//const char* charraceFormat = "uxxxxxxxxxxxuusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //322 - 332.11403
const char* charraceFormat   = "uxxxxxxxxxxxuuxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //335.12340 RaceName fr

//const char* charclassFormat = "uxuxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 309 - 322 - 332.11403
const char* charclassFormat   = "uxuxxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 335.12340 Fr

//const char* creaturefamilyFormat = "ufufuuuuuxsxxxxxxxxxxxxxxxxx"; // 332.11403 enGB
const char* creaturefamilyFormat   = "ufufuuuuuxxxsxxxxxxxxxxxxxxx"; // 335.12340 Fr

//const char* mapentryFormat = "usuxusxxxxxxxxxxxxxxxxusxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxuxuffxuxu"; // 332 enGB
const char* mapentryFormat   = "usuxuxxsxxxxxxxxxxxxxxuxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxuxuffxuxu"; // 335.12340 Fr (Brz)

								    //         11111111112222222222333333333344444444445555555555666         
                                    //12345678901234567890123456789012345678901234567890123456789012
const char* VehicleEntryFormat     = "uuffffuuuuuuuufffffffffffffffssssfufuuuu";  // 3.3.2.11403 (Crash), idem 335.12340 Fr (Brz)
const char* vehicleseatentryFormat = "uuuffffffffffuuuuuufffffffuuufffuuuuuuuffuuuuuxxxxxxxxxxxx"; // 3.3.2.11403 (Crash), idem 335.12340 Fr (Brz)

const char* itemrandomsuffixformat = "uxxxxxxxxxxxxxxxxxxuuuuuuuuux"; // 332.11403 - idem 335.12340 Fr

const char * durabilityqualityFormat = "uf"; // 335.12340 (Crash)
const char* bankslotpriceformat = "uu"; // 335.12340 (Crash)
const char * durabilitycostsFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu"; // 332.11403 - idem 335.12340 Fr
//const char* barbershopstyleFormat="uusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuu";  // 332.11403
const char* barbershopstyleFormat  ="uuxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuu";  // 335.12340 Fr
const char* gtfloatformat = "f"; // 335.12340 (Crash)
const char* spellshapeshiftformformat = "uxxxxxxxxxxxxxxxxxxxxxxxxxxxuuuuuuu"; // 332.11403 - idem 335.12340 Fr
const char* dbcsummonpropertiesFormat = "uuuuuu"; // 335.12340
const char* dbcspellrunecostFormat = "uuuuu"; // 332.11403 - idem 335.12340 Fr
 
//const char* AchievementFmt = "uuuusxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxuuuuusxxxxxxxxxxxxxxxxuu"; // 309 - 322 - 332.11403
const char* AchievementFmt =   "uuuuxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxuuuuuxxsxxxxxxxxxxxxxxuu"; // 335.12340 Fr
//const char* AchievementCriteria = "uuuuuuuuusxxxxxxxxxxxxxxxxuuxux"; // 332.11403
const char* AchievementCriteria   = "uuuuuuuuuxxsxxxxxxxxxxxxxxuuxux"; // 335.12340 Fr
//const char* AchievementCategory = "uusxxxxxxxxxxxxxxxxu"; // 3.0.9 - 322 - 332.11403
const char* AchievementCategory   = "uuxxsxxxxxxxxxxxxxxu"; // 335.12340 Fr
const char* WorldMapOverlayfmt="uxuxxxxxxxxxxxxxx"; // 332.11403 - idem 335.12340 Fr

                                   //            1111111111222
                                   //   1234567890123456789012
//const char* WorldSafeLocsStoreFormat = "uufffsxxxxxxxxxxxxxxxx"; // 332.11403 - Les cimetieres
const char* WorldSafeLocsStoreFormat   = "uufffxxsxxxxxxxxxxxxxx"; // 335.12340 Fr
const char* chatchannelformat = "uuxxxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 332.11403 - idem 335.12340 Fr

//                                                                                                      >|< SpellEffect (av 322)     >|< // debug des auras/spellEffect
//const char* spellentryFormat = "uuuuuuuuuuuxuxuxuuuuuuuuuuuuuuuuuuuuiuuuuiuuuuufuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuuuffffffiiiiiiuuuuuuuuuuuuuuufffuuuuuuuuuuuuuuufffuuuuuuuuuuuuuusxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxuuuuuuuuuuuufffuuuuuiuxxxxxxxu"; //332.11403
//const char* spellentryFormat = "uuuuuuuuuuuxuxuxuuuuuuuuuuuuuuuuuuuuiuuuuiuuuuufuuuuuuuuuuuuuuuuuuuuiuuuuuuuufffiiiiiiuuuuuuuuuuuuuuufffuuuuuuuuuuuuuuufffuuuuuuuuuuuuuuxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxuuuuuuuuuuuufffuuuuuiuxxxxxxxu"; //  335.12340 Fr
const char* spellentryFormat   = "uuuuuuuuuuuxuxuxuuuuuuuuuuuuuuuuuuuuiuuuuiuuuuufuuuuuuuuuuuuuuuuuuuuiuuuuuuuufffiiiiiiuuuuuuuuuuuuuuufffuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxuuuuuuuuuuuufffuuuuuiuxxuxxxxu"; //  335.12340 enGB

// DBComp 332.11403               uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuufuuuuuuuuuuuuiuuuuuuuiiuuuuuuuusssuufffiiiuuuuuuuuuuuuuuuuuufffuuuuuuiiiiuuiiufffiuuiiuiiuuuuuusuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuuuuuuuuuuuuuifffuuuuuiuuuifffiu   // 332 DBComp 
// DBComp 322                     uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuufuuuuuuuuuuuuiuuuuuuuiiuuuuuuuusssuufffiiiuuuuuuuuuuuuuuuuuufffuuuuuuiiiiuuiiufffiuuiiuiiuuuuuusuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuuuuuuuuuuuuuifffuuuuuiuuuifffs    // 322 DBComp
//const char* spellentryFormat = "uuuuuuuuuuuxuxuxuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuuuffffffiiiuuuuuuuuuuuuuuuuuufffuuuuuuuuuuuuuuufffuuuuuuuuuuuuuusxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxsxxxxxxxxxxxxxxxxuuuuuuuuuuuufffuuuuuuuuxxxxxx";  //322
//                                123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
//                                         111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999000000000011111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334
//                                                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112222222222222222222222222222222222222222222   
const char* XpQuestformat = "uxuuuuuuuux";
const char* QuestFactRewformat = "uuuuuuuuuuu";
const char* DestructibleModelDataFormat = "uxxuxxxuxxxuxxxuxxx";
// Arcemu
const char* wmoareaformat = "uiiixxxxxuuxxxxxxxxxxxxxxxxx";
//---------------
char DBCPath[1024];
char RSDBCPath[1024];

template<class T>
bool loader_stub(const char *_dbcpath, const char * filename, const char * format, bool ind, T& l, bool loadstrs)
{
	char _dbcpathfile[1024];
	sprintf(_dbcpathfile,"%s/%s",_dbcpath,filename);
	Log.Notice("DBC", "Loading %s.", _dbcpathfile);
	return l.Load(_dbcpathfile, format, ind, loadstrs);
}

#define LOAD_DBC(_dbcpath, filename, format, ind, stor, strings) if(!loader_stub(_dbcpath, filename, format, ind, stor, strings)) { return false; } 

bool LoadRSDBCs()
{
	char _RSDBCPath[MAX_PATH];
	memset(_RSDBCPath,0x00,MAX_PATH);
	strncpy(_RSDBCPath,RSDBCPath,MAX_PATH-10); // secu
	
	LOAD_DBC(_RSDBCPath,"AreaTable.dbc", areatableFormat, true, dbcArea, true);
	LOAD_DBC(_RSDBCPath,"ChatChannels.dbc", chatchannelformat, true, dbcChatChannels, false);
	return true;
}

bool LoadDBCs()
{
    char _DBCPath[MAX_PATH];
	memset(_DBCPath,0x00,MAX_PATH);
	strncpy(_DBCPath,DBCPath,MAX_PATH-10); // secu

	LOAD_DBC(_DBCPath,"AreaGroup.dbc", AreaGroupFormat, true, dbcAreaGroup, true);
	LOAD_DBC(_DBCPath,"AreaTrigger.dbc", AreaTriggerFormat, true, dbcAreaTrigger, true);
	LOAD_DBC(_DBCPath,"CharTitles.dbc", CharTitlesEntryfmt, true, dbcCharTitlesEntry, false);
	LOAD_DBC(_DBCPath,"ItemSet.dbc", ItemSetFormat, true, dbcItemSet, true);
	LOAD_DBC(_DBCPath,"Lock.dbc", LockFormat, true, dbcLock, false);
	LOAD_DBC(_DBCPath,"EmotesText.dbc", EmoteTextEntryFormat, true, dbcEmoteEntry, false);
	LOAD_DBC(_DBCPath,"SkillLineAbility.dbc", SkillLineSpellFormat, false, dbcSkillLineSpell, false);
	LOAD_DBC(_DBCPath,"SkillLine.dbc", SkillLineEntryFormat, true, dbcSkillLine, true);
		
	LOAD_DBC(_DBCPath,"Spell.dbc", spellentryFormat, true, dbcSpell, true);
	LOAD_DBC(_DBCPath,"SpellItemEnchantment.dbc", EnchantEntryFormat, true, dbcEnchant, true);
	LOAD_DBC(_DBCPath,"SpellCastTimes.dbc", spellcasttimeFormat, true, dbcSpellCastTime, false);
	LOAD_DBC(_DBCPath,"SpellRadius.dbc", spellradiusFormat, true, dbcSpellRadius, false);
	LOAD_DBC(_DBCPath,"SpellRange.dbc", spellrangeFormat, true, dbcSpellRange, false);
	LOAD_DBC(_DBCPath,"SpellDuration.dbc", spelldurationFormat, true, dbcSpellDuration, false);
	LOAD_DBC(_DBCPath,"SpellRuneCost.dbc", dbcspellrunecostFormat, true, dbcSpellRuneCost, false);
	LOAD_DBC(_DBCPath,"SpellShapeshiftForm.dbc", spellshapeshiftformformat, true, dbcSpellShapeshiftForm, false);

	LOAD_DBC(_DBCPath,"GemProperties.dbc", GemPropertyEntryFormat, true, dbcGemProperty, false);
	LOAD_DBC(_DBCPath,"GlyphProperties.dbc", GlyphPropertyEntryFormat, true, dbcGlyphProperty, false);

	LOAD_DBC(_DBCPath,"ItemRandomProperties.dbc", randompropsFormat, true, dbcRandomProps, false);
	LOAD_DBC(_DBCPath,"ItemExtendedCost.dbc", itemextendedcostFormat, true, dbcItemExtendedCost, false);

	LOAD_DBC(_DBCPath,"Talent.dbc", talententryFormat, true, dbcTalent, false);
	LOAD_DBC(_DBCPath,"TalentTab.dbc", talenttabentryFormat, true, dbcTalentTab, false);

	LOAD_DBC(_DBCPath,"AreaTable.dbc", areatableFormat, true, dbcArea, true);
	LOAD_DBC(_DBCPath,"FactionTemplate.dbc", factiontemplatedbcFormat, true, dbcFactionTemplate, false);
	LOAD_DBC(_DBCPath,"Faction.dbc", factiondbcFormat, true, dbcFaction, true);
	LOAD_DBC(_DBCPath,"TaxiNodes.dbc", dbctaxinodeFormat, false, dbcTaxiNode, true);
	LOAD_DBC(_DBCPath,"TaxiPath.dbc", dbctaxipathFormat, false, dbcTaxiPath, false);
	LOAD_DBC(_DBCPath,"TaxiPathNode.dbc", dbctaxipathnodeFormat, false, dbcTaxiPathNode, false);
	LOAD_DBC(_DBCPath,"CreatureSpellData.dbc", creaturespelldataFormat, true, dbcCreatureSpellData, false);
	LOAD_DBC(_DBCPath,"CreatureFamily.dbc", creaturefamilyFormat, true, dbcCreatureFamily, true);
	LOAD_DBC(_DBCPath,"ChrRaces.dbc", charraceFormat, true, dbcCharRace, true);
	LOAD_DBC(_DBCPath,"CreatureDisplayInfo.dbc", creaturedisplayFormat, true, dbcCreatureDisplayInfo, true);	
	LOAD_DBC(_DBCPath,"ChrClasses.dbc", charclassFormat, true, dbcCharClass, true);
	LOAD_DBC(_DBCPath,"Map.dbc", mapentryFormat, true, dbcMap, true);
	LOAD_DBC(_DBCPath,"AuctionHouse.dbc", auctionhousedbcFormat, true, dbcAuctionHouse, false);
	LOAD_DBC(_DBCPath,"ItemRandomSuffix.dbc", itemrandomsuffixformat, true, dbcItemRandomSuffix, false);
	LOAD_DBC(_DBCPath,"gtCombatRatings.dbc", gtfloatformat, false, dbcCombatRating, false);
	LOAD_DBC(_DBCPath,"ChatChannels.dbc", chatchannelformat, true, dbcChatChannels, false);
	LOAD_DBC(_DBCPath,"DurabilityQuality.dbc", durabilityqualityFormat, true, dbcDurabilityQuality, false);
	LOAD_DBC(_DBCPath,"DurabilityCosts.dbc", durabilitycostsFormat, true, dbcDurabilityCosts, false);
	LOAD_DBC(_DBCPath,"BankBagSlotPrices.dbc", bankslotpriceformat, true, dbcBankSlotPrices, false);
	LOAD_DBC(_DBCPath,"StableSlotPrices.dbc", bankslotpriceformat, true, dbcStableSlotPrices, false);
	LOAD_DBC(_DBCPath,"SummonProperties.dbc", dbcsummonpropertiesFormat, true, dbcSummonProperties, false);

	LOAD_DBC(_DBCPath,"BarberShopStyle.dbc", barbershopstyleFormat, true, dbcBarberShopStyle, true);

	LOAD_DBC(_DBCPath,"CurrencyTypes.dbc", currencyTypesFormat, true, dbcCurrencyTypes, false);

	// Spéciales
	LOAD_DBC(_DBCPath,"gtBarberShopCostBase.dbc", gtfloatformat, false, dbcBarberShopPrices, false);	
	LOAD_DBC(_DBCPath,"gtChanceToMeleeCrit.dbc", gtfloatformat, false, dbcMeleeCrit, false);
	LOAD_DBC(_DBCPath,"gtChanceToMeleeCritBase.dbc", gtfloatformat, false, dbcMeleeCritBase, false);
	LOAD_DBC(_DBCPath,"gtChanceToSpellCrit.dbc", gtfloatformat, false, dbcSpellCrit, false);
	LOAD_DBC(_DBCPath,"gtChanceToSpellCritBase.dbc", gtfloatformat, false, dbcSpellCritBase, false);
	LOAD_DBC(_DBCPath,"gtRegenMPPerSpt.dbc", gtfloatformat, false, dbcManaRegenBase, false); //it's not a mistake.
	LOAD_DBC(_DBCPath,"gtOCTRegenMP.dbc", gtfloatformat, false, dbcManaRegen, false); //it's not a mistake.
	LOAD_DBC(_DBCPath,"gtRegenHPPerSpt.dbc", gtfloatformat, false, dbcHPRegenBase, false); //it's not a mistake.
	LOAD_DBC(_DBCPath,"gtOCTRegenHP.dbc", gtfloatformat, false, dbcHPRegen, false); //it's not a mistake.

	// Cimetieres
	LOAD_DBC(_DBCPath,"WorldSafeLocs.dbc",WorldSafeLocsStoreFormat,true,dbcWorldSafeLocsStore,true);

	// HearthStone
	LOAD_DBC(_DBCPath,"Achievement.dbc", AchievementFmt,true, dbcAchievement,true);
	LOAD_DBC(_DBCPath,"Achievement_Criteria.dbc", AchievementCriteria,true,dbcAchivementCriteria,true);
	LOAD_DBC(_DBCPath,"Achievement_Category.dbc", AchievementCategory,true,dbcAchievementCategory,true);
	LOAD_DBC(_DBCPath,"Vehicle.dbc", VehicleEntryFormat, true, dbcVehicle, true);
	LOAD_DBC(_DBCPath,"VehicleSeat.dbc", vehicleseatentryFormat, true, dbcVehicleSeat, true);
	LOAD_DBC(_DBCPath,"WorldMapOverlay.dbc", WorldMapOverlayfmt, true, dbcWorldMapOverlay, true);
	LOAD_DBC(_DBCPath,"DestructibleModelData.dbc",DestructibleModelDataFormat, true, dbcDestructibleModelDataEntry, false);
	//-----------
	// Mangos
	LOAD_DBC(_DBCPath,"QuestXP.dbc", XpQuestformat, true, dbcQuestXPLevel, false );
	LOAD_DBC(_DBCPath,"QuestFactionReward.dbc", QuestFactRewformat, true, dbcQuestFactionReward, false);
	
	LOAD_DBC(_DBCPath,"WMOAreaTable.dbc", wmoareaformat, true, dbcWMOAreaTable, false );
	return true;
}

