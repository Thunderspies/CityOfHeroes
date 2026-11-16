// This file contains per-map info.  Broadly, this is stuff designers
// care about, like level ranges, as opposed to maps.db that contains
// stuff that programmers care about.
//
// This contains the specifications on what mission doors, contacts,
// touch locations, etc. should exist in each map.  Both maps and tasks
// are checked against this info.

// Outbreak
Map City_00_01.txt
{
	MissionMinLevel 1
	MissionMaxLevel 10
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 1

	ZoneScripts scriptdefs/Tutorial_Level.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors TutorialMission
	PersistentNPCs Officer_Parks, Detective_Wright, Sergeant_Hicks, Doctor_Miller, Lt_MacReady, Paragon_SWAT, Officer_Flint, Professor_Hoffman
	VisitLocations TutorialConningPlaque4, TutorialConningPlaque2, TutorialConningPlaque1, TutorialPlaqueX, 
	EncounterLayouts Single, CenteredAround, Duo, Encounter, 
	VillainGroups Contaminated, 
}

// Atlas Park
Map City_01_01.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 10

	ZoneScripts scriptdefs/LevelOne_LevelUpReminder.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, AbandonedOffice, Warehouse, AbandonedWarehouse, Sewers, Caves, Tech, 911_AP_Family_Jewels, 911_AP_First_National, 911_AP_Exchange, PPD_APdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, AP_PP_Office, AP_PP_AbandonedOffice, AP_PP_Sewers, AP_PP_Caves, AP_PP_Manhole, AP_PP_AbandonedWarehouse, AP_PP_Warehouse, AP_TP_Caves, AP_TP_AbandonedOffice, AP_TP_Office, AP_TP_Sewers, AP_TP_Tech, AP_TP_Manhole, AP_TP_AbandonedWarehouse, AP_TP_Warehouse, AP_HW_Office, AP_HW_AbandonedOffice, AP_HW_Tech, AP_HW_Sewers, AP_HW_Truck, AP_HW_Manhole, AP_HW_AbandonedWarehouse, AP_HW_Warehouse, AP_D_AbandonedOffice, AP_D_Office, AP_D_Sewers, AP_D_Tech, AP_D_Tech, AP_D_Warehouse, AP_D_Manhole, AP_D_Truck, AP_D_AbandonedWarehouse, AP_AI_AbandonedWarehouse, AP_AI_AbandonedOffice, AP_AI_Warehouse, AP_AI_Sewers, AP_AI_Sewers, AP_AI_Tech, AP_AI_Manhole, AP_AI_Truck, AP_MH_Mission4, AP_OF_M3, RS_I_1, AP_AT_M4, AP_SC_M4, OGT_ShiningStarsBaseOne, OmegaTeamMemorial, SigStory2PhalanxHQ
	PersistentNPCs FreedomCorp_City_01_01, Tony_Kord, Laurence_Mansfield, Jose_Brogan, FreedomCorp_City_01_01b, Susan_Davies, Security_Chief_01, MsLiberty, FreedomCorp_City_01_03, Iris_Parker, SuperGroupRegistrar, City_Representative, Sarah_Juarez, Henry_Peter_Wong, Azuria, Rick_Davies, Sarah_Peters, Security_Chief_04, Jonathan_Smythe, FreedomCorp_City_01_01c, Charlie, NotorietyContact6, Architect_Studio_Manager_H_AP, Architect_Greeter_AP, Architect_Instructor_AP, Architect_ComputerTech_AP, Architect_AsstManager_AP, Architect_MedicalTech_AP, Architect_VenderTech_AP,OGTH_Dillo_Act1, OGTH_Flambeaux_Act1, OGTH_Grym_Act1, OGTH_Proton_Act1, OGTH_Twinshot_Act1, Officer_Nelson_HabashyMish2, Wavelength, TUNNEL_Paragon, AntonioNashSSA2_2
	VisitLocations Statue1, Patrol_Hard_5_City_01_01, Patrol_Hard_4_City_01_01, Patrol_Hard_3_City_01_01, Patrol_Hard_2_City_01_01, Patrol_Hard_1_City_01_01, Patrol_Easy_5_City_01_01, Patrol_Easy_4_City_01_01, Patrol_Easy_3_City_01_01, Patrol_Easy_2_City_01_01, Patrol_Easy_1_City_01_01,  
	//Plaques CitizenCrimeAct2, AxisAttack1, CitizenCrimeAct1, MightForRightAct1, PsychicFeedbackLoop1,
	EncounterLayouts AroundDoor, Single, Snatch, Ambush, AroundVandalism, Around, 
	VillainGroups Clockwork, Hellions, Vahzilok, Skulls, Arachnos
}

// Kings Row
Map City_01_02.txt
{
	MissionMinLevel 5
	SpawnOverrideMinLevel 5

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef
	ZoneScripts scriptdefs/lua/Paladin_Zone_Event.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors KR_IA_AbandonedOffice, KR_TG_Warehouse_Shauna_Stockwell_Mission4, Office, AbandonedOffice, AbandonedWarehouse, Warehouse, Sewers, Caves, Tech, PPD_KRdoor1, 911_KR_Pwn_Shop, 911_KR_First_National, 911_KR_Exchange, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, OGT_ShiningStarsBaseTwo, KR_A_Office, KR_FP_Office, KR_TG_Office, KR_IA_Office, KR_KGW_Office, KR_A_AbandonedOffice, KR_HP_AbandonedOffice, KR_KGW_AbandonedOffice, KR_A_Tech, KR_TG_Tech, KR_HP_Tech, KR_TG_AbandonedWarehouse, KR_IA_AdandonedWarehouse, KR_KGW_AbandonedWarehouse, KR_FP_Warehouse, KR_TG_Warehouse, KR_IA_Warehouse, KR_RR_Warehouse, KR_A_Caves, KR_A_Caves, KR_A_Sewers, KR_FP_Sewers, KR_TG_Sewers, KR_IA_Sewers, KR_RR_Sewers MissionFromSkullsHideout
	PersistentNPCs Genevieve_Sanders, Juan_Jimenez, Vic_Johansson, Paula_Dempsey, Blue_Steel, Security_Chief_03, Samuel_Pierce, Ron_Hughes, Linda_Summers, NotorietyContact2, OGTH_Dillo_Act2
	VisitLocations Patrol_Easy_5_City_01_02, Patrol_Easy_1_City_01_02, Patrol_Easy_2_City_01_02, Patrol_Easy_3_City_01_02, Patrol_Easy_4_City_01_02, Patrol_Hard_5_City_01_02, Patrol_Hard_4_City_01_02, Patrol_Hard_3_City_01_02, Patrol_Hard_2_City_01_02, Patrol_Hard_1_City_01_02,  
	//Plaques AxisAttack3, AxisAttack4, PsychicFeedbackLoop3, PsychicFeedbackLoop4, PsychicFeedbackLoop5,
	EncounterLayouts Rumble, RumbleRanged, Duo, Barrel, guard, Beatup, PigMiddle, BrawlShipRec, TurkeyShoot, Architect3Chat, CenteredAround, DuoWatch, PurseSnatch, Apprehend
	VillainGroups CircleOfThorns, Skulls, TheLost, Vahzilok, Clockwork
}

// Galaxy City
Map City_01_03.txt
{
	MissionMinLevel 1
	MissionMaxLevel 10
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 10

	ZoneScripts scriptdefs/Teleport_To_Atlas.scriptdef

	MissionsAllowed None
	PlayersAllowed Hero
	MissionDoors Office, AbandonedWarehouse, Warehouse, Caves, Sewers, Tech, 911_GC_Exchange, 911_GC_Family_Jewels, 911_GC_First_National, PPD_GCdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, GC_FC_Office, GC_FC_Sewers, GC_FC_AbandonedWarehouse, GC_FC_Warehouse
	PersistentNPCs FreedomCorp_City_01_03c, FreedomCorp_City_01_03b, FreedomCorp_City_01_03, Charlie_Sparks, Security_Chief_02, Kiros_Nandelu, Derek_Amberson, Clarence_Jackson, Back_Alley_Brawler, Maurice_Feldon, SuperGroupRegistrar, Paco_Sanchez, Kip_Cantorum, Rebecca_Brinell, Caitlin_Murray, Gregor_Richardson, Rachel_Torres, NotorietyContact1
	VisitLocations Patrol_Easy_1_City_01_03, Patrol_Easy_2_City_01_03, Patrol_Easy_3_City_01_03, Patrol_Easy_4_City_01_03, Patrol_Easy_5_City_01_03, Patrol_Hard_1_City_01_03, Patrol_Hard_2_City_01_03, Patrol_Hard_3_City_01_03, Patrol_Hard_4_City_01_03, Patrol_Hard_5_City_01_03, Statue2, 
	//Plaques AxisAttack2, CitizenCrimeAct3, MightForRightAct2, MightForRightAct3, PsychicFeedbackLoop2, 
	EncounterLayouts Snatch, Single, AroundVandalism, Duo, Encounter, Around, 
	VillainGroups Hellions, Vahzilok, Clockwork, 
}
// Echo: Galaxy City
Map City_01_04.txt
{
	MissionMinLevel 1
	MissionMaxLevel 10
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 10

	//ZoneScripts scriptdefs/City_01_03_Teleport.scriptdef

	MissionsAllowed None
	PlayersAllowed Hero
	//MissionDoors Office, AbandonedWarehouse, Warehouse, Caves, Sewers, Tech, 911_GC_Exchange, 911_GC_Family_Jewels, 911_GC_First_National, PPD_GCdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, GC_FC_Office, GC_FC_Sewers, GC_FC_AbandonedWarehouse, GC_FC_Warehouse, 
	//PersistentNPCs FreedomCorp_City_01_03c, FreedomCorp_City_01_03b, FreedomCorp_City_01_03, Charlie_Sparks, Security_Chief_02, Kiros_Nandelu, Derek_Amberson, Clarence_Jackson, Back_Alley_Brawler, Maurice_Feldon, SuperGroupRegistrar, Paco_Sanchez, Kip_Cantorum, Rebecca_Brinell, Caitlin_Murray, Gregor_Richardson, Rachel_Torres, NotorietyContact1
	VisitLocations Patrol_Easy_1_City_01_03, Patrol_Easy_2_City_01_03, Patrol_Easy_3_City_01_03, Patrol_Easy_4_City_01_03, Patrol_Easy_5_City_01_03, Patrol_Hard_1_City_01_03, Patrol_Hard_2_City_01_03, Patrol_Hard_3_City_01_03, Patrol_Hard_4_City_01_03, Patrol_Hard_5_City_01_03, Statue2, 
	//Plaques AxisAttack2, CitizenCrimeAct3, MightForRightAct2, MightForRightAct3, PsychicFeedbackLoop2, 
	EncounterLayouts Snatch, Single, AroundVandalism, Duo, Encounter, Around, 
	VillainGroups Hellions, Vahzilok, Clockwork, 
}
// Steel Canyon
Map City_02_01.txt
{
	MissionMinLevel 10
	SpawnOverrideMinLevel 10

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/SteelCanyon_OnEnterGrantPower.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, AbandonedOffice, Warehouse, AbandonedWarehouse, Caves, Sewers, Tech, 911_SC_Dianes_Diamonds, 911_SC_First_National, 911_SC_Exchange, PPD_SCdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, InventionTutorial, Architect, OGTM_Arc3_M1, OGTM_Arc3_M2, Lockhart_Mission3_Part2, GrahamEastonMuseum, GrahamEastonTsooWarehouse
	PersistentNPCs Kyle_Peck, Mutant_City_02_01, Tech_City_02_01c, Guy_Denson, Mannequin_Tech_Female, Science_City_02_01b, Shelly_Knowles, Natural_City_02_01, Virginia_Hoffman, Security_Chief_05, Fareed_Abdullah, Olivia_Chung, Wes_Schnabel, Mannequin_Mut_Male, IconEmployee_City_02_01c, Tech_City_02_01, Tech_City_02_01b, Positron_2, Mannequin_Nat_Female, Valkyrie, Tom_Bowden, FreedomCorp_City_02_02b, Mannequin_Sci_Male, IconEmployee_City_02_01b, Mannequin_Nat_Male, Willy_Starbuck, Wyatt_Anderson, Security_Chief_17, Science_City_02_01, Mannequin_Sci_Female, IconEmployee1, Hugo_Redding, Natural_City_02_01c, Magic_City_02_01c, IconEmployee_City_02_01a, Mutant_City_02_01c, Magic_City_02_01, Mutant_City_02_01b, Mannequin_Mut_Female, Mannequin_Tech_Male, Security_Chief_06, Magic_City_02_01b, Wilson_Zucco, Natural_City_02_01b, Colleen_Saramago, FreedomCorp_City_02_02c, Dr_Trevor_Seaborn, Carlos_Herrera, Alfonse_Rubel, Mannequin_Mage_Male, Science_City_02_01c, FreedomCorp_City_02_02, Athena_Currie, NotorietyContact7, Admissions_Lenk, Counselor_Pires, Professor_Boram, Professor_Chandler, Professor_Topffer, Professor_Taylor, Montague_Castanella, Latin_Student, OGTH_Grym_Act2, Laura_Lockhart, Colonel_Taggard
	VisitLocations Patrol_Hard_2_City_02_01, Patrol_Easy_3_City_02_01, Patrol_Easy_1_City_02_01, Patrol_Easy_5_City_02_01, Patrol_Easy_4_City_02_01, Patrol_Easy_2_City_02_01, Patrol_Hard_5_City_02_01, Patrol_Hard_4_City_02_01, Patrol_Hard_1_City_02_01, Patrol_Hard_3_City_02_01, Invention_Course_Syllabus, Invention_Bookcase, Invention_Library_Terminal, Invention_Tech_Computer, Invention_Storage_Cylinders, Invention_Bulletin, InventionTutorialWB1, InventionTutorialWB2, InventionTutorialWB3, InventionTutorialWB4
	//Plaques WarOnDrugs1, Superadine1, Superadine2, HeroCorps1,
	EncounterLayouts Around, Single, Snatch, Duo, Encounter, AroundVandalism, GroupedDirection, Rumble, 
	VillainGroups Tsoo, CircleofThorns, Clockwork, OutCasts, Vahzilok, Council,  
}

// Skyway City
Map City_02_02.txt
{
	MissionMinLevel 10
	SpawnOverrideMinLevel 10

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, AbandonedOffice, Caves, Warehouse, Caves, Tech, Sewers, 911_SW_Damicos_Pawn, 911_SW_First_National, 911_SW_Exchange, PPD_SWdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, SkywayHTF, SigArcOneIssue1M1, SigArcOneIssue1M2, SigArcOneIssue1M3
	PersistentNPCs Mutant_City_02_01, Tech_City_02_01c, Mutant_City_02_01b, Lorenzo_DiCosta, Mark_Freeman, Mynx, Science_City_02_01b, Science_City_02_01c, Thao_Ku, Natural_City_02_01, Yancy_Rhymes, Security_Chief_07, Maggie_Greene, Tech_City_02_01, Karen_Parker, Tech_City_02_01b, Everett_Daniels, Haley_Philips, Yolanda_Baker, FreedomCorp_City_02_02b, Magic_City_02_01, Pavel_Garnier, Security_Chief_17, Science_City_02_01, Kong_Bao, Ann-Marie_Engles, Cho_Ge, Tristan_Caine, Corey_McCann, Natural_City_02_01c, Magic_City_02_01c, Warren_Trudeau, Synapse, Andre_Jimenez, Sgt_Suzanne_Bernhard, Vitaly_Cherenko, Jerry_Kazatsky, Jake_Montoya, Natural_City_02_01b, Kira_Lange, FreedomCorp_City_02_02c, Jill_Pastor, Mutant_City_02_01c, Hiro_Takashi, Magic_City_02_01b, Security_Chief_08, The_Can_Man, FreedomCorp_City_02_02, Carla_Brunelli, Juliana_Nehring, Sanjay_Chandra, NotorietyContact8, Theoden, Theoden_Guard
	VisitLocations Patrol_Hard_5_City_02_02, Patrol_Hard_4_City_02_02, Patrol_Hard_3_City_02_02, Patrol_Hard_2_City_02_02, Patrol_Hard_1_City_02_02, Patrol_Easy_5_City_02_02, Patrol_Easy_4_City_02_02, Patrol_Easy_3_City_02_02, Patrol_Easy_2_City_02_02, Patrol_Easy_1_City_02_02,  
	//Plaques WarOnDrugs2, WarOnDrugs3, Superadine3, HeroCorps2,
	EncounterLayouts AroundDoor, Single, Snatch, Duo, Encounter, AroundVandalism, Around, 
	VillainGroups Clockwork, TheLost, Trolls, 
}

// Paragon Dance Party
Map City_02_03.txt
{
	MissionsAllowed All
	PlayersAllowed Hero, Villain
	ZoneScripts scriptdefs/PDP_Friendly.scriptdef
}

// Midnighter Club
Map Coop_02_01.txt
{
	MissionMinLevel 10
	MissionMaxLevel 50
	SpawnOverrideMinLevel 10
	SpawnOverrideMaxLevel 50

	MissionsAllowed All
	PlayersAllowed Hero, Villain
	ZoneScripts scriptdefs/MidnighterClub_Friendly.scriptdef
	ZoneScripts scriptdefs/MidnighterZoneHeroes_Teleport.scriptdef
	ZoneScripts scriptdefs/MidnighterZoneVillain_Teleport.scriptdef
	ZoneScripts scriptdefs/MidnighterZoneVillain_Teleport.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Midnighter.scriptdef

	MissionDoors 
	PersistentNPCs  Midnighter_Greeter
	VisitLocations  
	EncounterLayouts 
	VillainGroups
}



// Pocket D
Map City_02_04.txt
{
	MissionMinLevel 1
	MissionMaxLevel 50
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 50

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Primal, Praetorian
	ZoneScripts scriptdefs/PDP_Friendly.scriptdef
	ZoneScripts scriptdefs/PocketD_SkiResort.scriptdef
	ZoneScripts scriptdefs/PocketD_Teleport.scriptdef
	ZoneScripts scriptdefs/PocketD_MonkeyFight.scriptdef
	ZoneScripts scriptdefs/PocketD_Slalom.scriptdef
	ZoneScripts scriptdefs/PocketD_Slalom_2.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Clubber.scriptdef

	MissionDoors Pocket_D, Pocket_D_Truck, Pocket_D_Manhole, Pocket_D_Door, Pocket_D_Portal, PkD_slope_Mission, PokD_ChaletDoor
	PersistentNPCs Event_Spring_06_DJ_Zero
	VisitLocations  
	EncounterLayouts 
	VillainGroups
}

// Faultline
Map City_02_05.txt
{
	MissionMinLevel 11
	SpawnOverrideMinLevel 15

	MissionsAllowed Hero
	PlayersAllowed Hero

	ZoneScripts scriptdefs/City_02_05_Teleport.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef

	MissionDoors FL_NO_Warehouse, Warehouse, FL_NO_Sewer, Sewer, FL_NO_AbandonedOffice, AbandonedOffice, FL_AS_AbandonedOffice, AbandonedOffice, FL_AS_AbandonedWarehouse, AbandonedWarehouse, FL_AS_Cave, Cave, FL_OF_AbandonedOffice, FL_OF_Boat, Boat, FL_OF_Cave, FL_OF_IOM_Turbine, FL_OF_Tech1, FL_OF_Tech2, FL_OF_Tech3, FL_OD_Arachnos, FL_OD_Arachnos_1, Arachnos_1, Arachnos, FL_OD_Warehouse, FL_OD_Arachnos_AgentG_M3, FL_OD_SubBase, FL_OD_SkyRaider_Warehouse, FL_OD_SubBase, FL_OD_IOM_Longbow_Flyer, FL_OD_AgentG_Warehouse, NotADoor, 911_FL_Exchange, 911_FL_First_National, 911_FL_Dianes_Diamonds, Architect, FL_AS_PosiTFBoat
	PersistentNPCs Mirror_Spirit, Jim_Temblor, Penelope_Yin, Agent_G, Doc_Delilah
	//Plaques WarOnDrugs5, Superadine5, HeroCorps6, HeroCorps7, Faultline1, Faultline2, Faultline3, Faultline4, Faultline5, Faultline6, Faultline7, InMemoriam
	VillainGroups Arachnos, Freakshow, Clockwork, FreedomCorps, Hydra, TheLost, ParagonPolice, SkyRaiders, Vahzilok
}



// Talos Island
Map City_03_01.txt
{
	MissionMinLevel 20
	SpawnOverrideMinLevel 20

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/GM-Adamastor.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, AbandonedOffice, Warehouse,  Council,  Caves, Tech, Sewers, CircleOfThorns, TI_CargoShip, 911_TI_Family_Jewels, 911_TI_First_National, 911_TI_Exchange, PPD_TIdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, Roy_Mission1, Roy_Mission2, Roy_Mission3, Roy_Mission5, Roy_Mission4, SigArcOneIssue2M1, SigArcOneIssue2M3
	PersistentNPCs Mutant_City_03_01c, Jim_Bell, Tech_City_03_01b, FreedomCorp_City_03_01b, Natural_City_03_02b, Science_City_03_02b, Manuel_Ruiz, Tech_City_03_01, John_Strobel, Joesef_Keller, Magic_City_03_01c, Eliza_Thorpe, Evan_DiTomasso, Luminary, Cain_Royce, Joshua_Stutz, Magic_City_03_01b, Science_City_03_02, Tyler_French, Claire_Childress, Tech_City_03_01c, Janelle_Irving, Lt_Col_Hugh_McDougal, Mutant_City_03_01, Mutant_City_03_01b, Jesse_Hobart, FreedomCorp_City_03_01, Piper_Irving, Vic_Garland, Alexander, Wendy_Klein, Bastion, Marvin_Weintraub, Annie_Coults, Oliver_Haak, Andrew_Fiore, Miriam_Bloechl, FreedomCorp_City_03_01c, Hinckley_Rasmussen, Security_Chief_10, Natural_City_03_02, Natural_City_03_02c, Betty_Abbot, Polly_Cooper, Robert_Koslowski, Science_City_03_02c, Security_Chief_09, Barry_Gosford, Andrea_Mitchell, Magic_City_03_01, NotorietyContact3, Architect_ComputerTech, Architect_AsstManager, Architect_MedicalTech, Special_Agent_Jenni_Adair, Field_Agent_Keith_Nance, Albert_Maurer, Mike_Bino, TUNNEL_Paragon,
	VisitLocations Patrol_Hard_5_City_03_01, Patrol_Hard_4_City_03_01, Patrol_Hard_3_City_03_01, Patrol_Hard_2_City_03_01, Patrol_Hard_1_City_03_01, Patrol_Easy_5_City_03_01, Patrol_Easy_4_City_03_01, Patrol_Easy_3_City_03_01, Patrol_Easy_2_City_03_01, Patrol_Easy_1_City_03_01, 
	//Plaques SpecialCouncil2, FreedomPhalanx3, SpankyRabinowitz2, SpankyRabinowitz3, 
	EncounterLayouts Rumble, AroundStore, Single, Snatch, Duo, Encounter, AroundVandalism, Around, CargoLookout, CargoRumbles
	VillainGroups Warriors, CircleOfThorns, Tsoo, Freakshow, DevouringEarth, BanishedPantheon, 
}

// Independence Port
Map City_03_02.txt
{
	MissionMinLevel 20
	SpawnOverrideMinLevel 22

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors AbandonedOffice, AbandonedWarehouse, Warehouse,  Council,  Sewers, Tech, IP_CargoShip, 911_IP_Pwn_Shop, 911_IP_First_National, 911_IP_Exchange, PPD_IPdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, IP_STF_Heli, IP_STF_Sub, IP_CC_Warehouse, Architect, AtlanticOceanHTF, TalosHTF, IP_PTF_Office1, IP_PTF_Heli2
	PersistentNPCs Justin_Greene, Mutant_City_03_01c, Mannequin_Tech_Female, IconEmployee_City_03_02d, Lorenzo_Tate, Tech_City_03_01b, FreedomCorp_City_03_01b, Natural_City_03_02b, Science_City_03_02b, Magic_City_03_01c, Collin_Larson, Tech_City_03_01, Magic_City_03_01, Mannequin_Mut_Male, Mannequin_Nat_Female, Georgia_Fields, Christine_Lansdale, Melanie_Peebles, Mannequin_Sci_Male, Mannequin_Nat_Male, Dr_Cheng, Science_City_03_02, Penelope_Yin_TF, Mannequin_Sci_Female, Magic_City_03_01b, IconEmployee_City_03_02e, Security_Chief_12, Mutant_City_03_01, Mutant_City_03_01b, Jake_Kim, Aurora_Borealis, IconEmployee_City_03_02f, FreedomCorp_City_03_01, Tech_City_03_01c, Dennis_Ewell, Mannequin_Mut_Female, Laurie_Pennington, Security_Chief_11, FreedomCorp_City_03_01c, Laura_Brunetti, Natural_City_03_02, Wilma_Peterson, Wanda_Travis, Amanda_Loomis, Karl_Bolger, Natural_City_03_02c, Oswald_Cuthbert, Kevin_Cordell, Wilson_Eziquerra, Science_City_03_02c, Justine_Kelly, Ashwin_Lannister, Rondel_Jackson, Mannequin_Mage_Male, Mannequin_Tech_Male, Kirsten_Woods, IconEmployee2, Jane_Hallaway, NotorietyContact9,
	VisitLocations Patrol_Easy_1_City_03_02, Patrol_Easy_2_City_03_02, Patrol_Easy_3_City_03_02, Patrol_Easy_4_City_03_02, Patrol_Easy_5_City_03_02, Patrol_Hard_1_City_03_02, Patrol_Hard_2_City_03_02, Patrol_Hard_3_City_03_02, Patrol_Hard_4_City_03_02, Patrol_Hard_5_City_03_02, 
	//Plaques SpecialCouncil1, FreedomPhalanx1, FreedomPhalanx2, SpankyRabinowitz1, 
	EncounterLayouts Around, Single, AroundVandalism, Duo, Encounter, GroupedDirection, Rumble, RumbleCatwalk, CargoLookout, CargoRumbles
	VillainGroups Tsoo, Council,  TheFamily, DevouringEarth, 
}

// Croatoa
Map City_03_03.txt
{
	MissionMinLevel 25
	SpawnOverrideMinLevel 28

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Cro_Fairy1, Cro_Fairy2, Cro_Fairy3, Cro_Fairy4, Cro_Fairy5, Cro_Fairy6, Cro_Fairy7, Cro_Fairy8, Cro_Fairy9, Cro_Salamanca_Warehouse, Cro_Salamanca_Office, Cro_Salamanca_Office, Cro_Salamanca_Sewers, Cro_BT_Caves, Cro_Lake_Caves, Cro_SR_Caves, Cro_NC_Caves, Cro_MW_Caves, Cro_SR_Tree,
	PersistentNPCs Gordon_Bower, Skipper_LeGrange, Kelly_Nemmers, Buck_Salinger, Katie_Hannon, War_Witch, Percy_Winkley, 
	//VisitLocations Patrol_Easy_1_City_03_02, Patrol_Easy_2_City_03_02, Patrol_Easy_3_City_03_02, Patrol_Easy_4_City_03_02, Patrol_Easy_5_City_03_02, Patrol_Hard_1_City_03_02, Patrol_Hard_2_City_03_02, Patrol_Hard_3_City_03_02, Patrol_Hard_4_City_03_02, Patrol_Hard_5_City_03_02, 
	//Plaques SpecialCouncil1, FreedomPhalanx1, FreedomPhalanx2, SpankyRabinowitz1, 
	EncounterLayouts Event_Tuatha, Event_Fey, Event_Cabal, Event_FirBolg, FeySquabble, CabalCaptured, FeyTargetPractice, FeyAmbush, FeyBattleLine, JackPatrol, FeyLoiter, FeyGuarding, FeyScaffold, FeyConference, FeyLoiter, CabalLoiter, Fey_CabalRngRumble, Fey_CabalRumble, FeyGuarding, FeyCeremony, TotemPull, FeyTrap, FeyTrees, FirTrees, WalkingTrees, TuathaCeremony, ForestClearing, InvasionForce, InvasionPrep1, InvasionPrep2, TuathaLoiter, TuathaTrees, WispLoiterDuo, WispPatrol, WispTrees, JackSleeping, JackPatrol, FirLoiter, 3WayFight, TomeReading, EochaiGroup, EochaiTree, FirBarnAttack, FirCabalStandOff, Coerce, FirBolgPatrol, FirPortal, FieldAmbush, FromAbove, Scarecrows, Vandalize, EochaiWorship, HaystackRave, NightIncursion, FarmerDigHole, FarmerPickAxe, PumpkinWheelbarrel, Repair, Weeding, WheelbarrelRocks, CabalLoiter, TuathaLoiter, TuathaHayStack, TuathaFields, FirHayStack, GhostsHaunt, GhostsLoiter, CabalHarass, Loiter, GhostHaunt, Loiter, SanityCheck, TotemPull, Trees, GhostHaunt, Emergence, Rumble, TotemPull, NPC, Loiter, Converse, Blocked, Blocking, CarHopping, Phone, Rummage, FromBuilding, Rampage, GhostPerch, FirCabalTalks, CabalDoorAmbush, CabalDeepDefense, CabalLoiter, CabalNightLights, CabalShoreDefense, CabalRedCapHostage, CabalFarming, CabalSkyPatrol, CabalGiantTree
	VillainGroups FirBolg, Tuatha, Cabal, RedCaps, CroatoaGhosts
}

// Cimerora
Map City_03_04.txt
{
	MissionMinLevel 35
	MissionMaxLevel 50
	SpawnOverrideMinLevel 20
	SpawnOverrideMaxLevel 50

	ZoneScripts scriptdefs/CimeroraZone_Friendly.scriptdef
	ZoneScripts scriptdefs/CimeroaZoneHeroes_Teleport.scriptdef
	ZoneScripts scriptdefs/CimeroaZoneVillain_Teleport.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Cimeroran.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Primal
	MissionDoors CP_AM_RomanCaves, CP_AM_Castellum_Quarter, CP_AM_Boat
	PersistentNPCs Imperious, Marcus_Valerius, Senator_Aquila, Cimerora_Antonia_Severa, Cimerora_Atticus, Cimerora_Old_Woman, Cimerora_Brutticus_Aurelius, Cimerora_Julia_Pria, Daedalus, Sister_Airlia, City_03_04_Nurse, Agent_Kwahu
//	VisitLocations  
//	EncounterLayouts 
	VillainGroups Romans, Romans_Good
}

// Founders' Falls
Map City_04_01.txt
{
	MissionMinLevel 30
	SpawnOverrideMinLevel 30

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, Caves, sewers, Tech, CircleOfThorns, Council, 911_FF_Dianes_Diamonds, 911_FF_First_National, 911_FF_Exchange, PPD_FFdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice, PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, SigArc4
	PersistentNPCs Security_Chief_15, Tina_Chung, Maxwell_Christopher, Peter_Stemitz, Infernal, Phillipa_Meraux, Cadao_Kestrel, Jenny_Firkins, Mannequin_Tech_Female, Anton_Sampson, Numina, Jose_Escalante, Janet_Kellum, Angus_McQueen, Penny_Preston, Mark_IV, Mannequin_Tech_Male, Mannequin_Mut_Male, IconEmployee3, IconEmployee_City_04_01g, IconEmployee_City_04_01i, Ginger_Yates, IconEmployee_City_04_01h, Mannequin_Nat_Female, Mannequin_Sci_Male, Mannequin_Nat_Male, Mannequin_Mut_Female, Indigo, Security_Chief_16, Agent_Six, Mannequin_Mage_Male, Mannequin_Sci_Female, Madeleine_Casey, James_Harlan, NotorietyContact10, GIFT_Liason, TUNNEL_Paragon,
	VisitLocations Patrol_Easy_1_City_04_01, Patrol_Easy_2_City_04_01, Patrol_Easy_4_City_04_01, Patrol_Hard_1_City_04_01, Patrol_Easy_3_City_04_01, Patrol_Hard_2_City_04_01, Patrol_Easy_5_City_04_01, Patrol_Hard_3_City_04_01, Patrol_Hard_4_City_04_01, Patrol_Hard_5_City_04_01, 
	//Plaques RiktiWar1, DimensionalBarrior1,
	EncounterLayouts Rumble, Around, Catwalk, Single, AroundVandalism, Duo, Encounter, Phalanx, GroupedDirection, 
	VillainGroups CircleOfThorns, Crey, Rikti, DevouringEarth, Council,  
}

// Brickstown
Map City_04_02.txt
{
	MissionMinLevel 30
	SpawnOverrideMinLevel 30

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/Lua/Prison_Break.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, AbandonedOffice, AbandonedWarehouse, Warehouse, Sewers,  Council,  Caves, Tech, Council, 911_BT_Damicos_Pawn, 911_BT_First_National, 911_BT_Exchange, PPD_BTdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice, PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, SigArcEp4M1, SigArcEp4M2, i24HeroArc1M2, i24HeroArc1M3, i24HeroArc1M5, i24HeroArc2M2, i24HeroArc2M3, i24HeroArc2M4, i24HeroArc3M3
	PersistentNPCs Merisel_Valenzuela, Neal_Kendrick, Security_Chief_14, Holsten_Armitage, Swan, Colleen_Nelson, Gordon_Stacy, Steven_Sheridan, Lou_Pasterelli, Security_Chief_13, Serafina, Manticore, Allison_King, NotorietyContact4
	VisitLocations Patrol_Hard_5_City_04_02, Patrol_Hard_4_City_04_02, Patrol_Hard_3_City_04_02, Patrol_Hard_2_City_04_02, Patrol_Hard_1_City_04_02, Patrol_Easy_5_City_04_02, Patrol_Easy_4_City_04_02, Patrol_Easy_3_City_04_02, Patrol_Easy_2_City_04_02, Patrol_Easy_1_City_04_02, 
	//Plaques DimensionalBarrier2, DimensionalBarrier3, 
	EncounterLayouts Phalanx, BigAround, Rumble, BrawlShipRec, Architect3Chat, Beatup, guard, Snatch, Breakout, 
	VillainGroups Crey, Prisoners, Freakshow, Council	
	VisitLocations Dead_Drop_01, Dead_Drop_02, Dead_Drop_03, Dead_Drop_04, Dead_Drop_05
}

// Peregrine Island
Map City_05_01.txt
{
	MissionMinLevel 40
	SpawnOverrideMinLevel 40

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, Tech, Portal, Warehouse, CircleOfThorns,  Council,  Caves, PI_CargoShip, 911_PI_Pwn_Shop, 911_PI_First_National, 911_PI_Exchange, PPD_PIdoor1, PoliceBandCaves, PoliceBandSewers, PoliceBandOffice ,PoliceBandAbandonedOffice, PoliceBandWarehouse, PoliceBandAbandonedWarehouse, PoliceBandCircleOfThorns, Safeguard, Architect, SG1A5M1, SG1A5M2, SA1E6M1, SA1E6M3
	PersistentNPCs Richard_Flagg, Harvey_Maylor, Ghost_Falcon, Maria_Jenkins, Tina_Macintyre, Unai_Kemen, Crimson, NotorietyContact5, Architect_ComputerTech, Architect_AsstManager, Architect_MedicalTech, FreedomCorp_Liason, TUNNEL_Paragon,
	VisitLocations Patrol_Easy_1_City_05_01, Patrol_Easy_2_City_05_01, Patrol_Easy_3_City_05_01, Patrol_Easy_4_City_05_01, Patrol_Easy_5_City_05_01, Patrol_Hard_1_City_05_01, Patrol_Hard_2_City_05_01, Patrol_Hard_3_City_05_01, Patrol_Hard_4_City_05_01, Patrol_Hard_5_City_05_01, 
	//Plaques NemesisSeizesControl1, 
	EncounterLayouts CircledAroundEX, Catwalk, Assault, Single, PersuadeDemo, Ambush, Encounter, Phalanx, Around, CargoLookout, CargoRumbles
	VillainGroups Malta, CircleOfThorns, Carnival, Nemesis, Rikti, DevouringEarth, 
}

// Ouroboros Hero
Map City_06_01.txt
{
	MissionMinLevel 25
	SpawnOverrideMinLevel 25

	ZoneScripts scriptdefs/Ouroboros_Hero_Lvl_Teleport.scriptdef
	ZoneScripts scriptdefs/Ouroboros_Hero_RejectVillains.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Mender.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Portal_Tesseract, Portal_Twilight, Portal_Lazarus, Portal_Pilgrim, Portal_Silos, Portal_Ramiel
	PersistentNPCs Mender_Silos, TwilightsSon, Mender_Lazarus, Pilgrim, OuroborosStore_01, Mender_Ramiel
	//Plaques 
	EncounterLayouts 
	VillainGroups 
}

// Ouroboros Event
Map Event_06_01.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1

	MissionsAllowed All
	PlayersAllowed Hero, Villain
//	MissionDoors Portal_Tesseract, Portal_Twilight, Portal_Lazarus, Portal_Pilgrim, Portal_Silos
//	PersistentNPCs Mender_Silos, TwilightsSon, Mender_Lazarus, Pilgrim, OuroborosStore_01
	//Plaques 
	EncounterLayouts 
	VillainGroups 

	ZoneScripts scriptdefs/Event0601SetLevel.scriptdef
	ZoneScripts scriptdefs/WeddingEvent.scriptdef
}

// Hero Lounge
Map City_06_02.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1

	MissionsAllowed Hero
	PlayersAllowed Hero
//	MissionDoors 
//	PersistentNPCs 
	//Plaques 
}


// Perez Park
Map Hazard_01_01.txt
{
	MissionMinLevel 9
	SpawnOverrideMinLevel 9

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Office, AbandonedOffice, Caves, Tech, Sewers
	VisitLocations Patrol_Hard_5_Hazard_01_01, Patrol_Hard_4_Hazard_01_01, Patrol_Hard_3_Hazard_01_01, Patrol_Hard_2_Hazard_01_01, Patrol_Hard_1_Hazard_01_01, Patrol_Easy_5_Hazard_01_01, Patrol_Easy_4_Hazard_01_01, Patrol_Easy_3_Hazard_01_01, Patrol_Easy_2_Hazard_01_01, Patrol_Easy_1_Hazard_01_01, 
	//Plaques AxisAttack5, CitizenCrimeAct4, CitizenCrimeAct5, MightForRightAct4, 
	EncounterLayouts Rumble, Ambush, Encounter, Around, Single, 
	VillainGroups CircleofThorns, Clockwork, Hydra, Skulls, Vahzilok, Hellions, TheLost, 
}

// The Hollows
Map Hazard_01_02.txt
{
	MissionMinLevel 5
	SpawnOverrideMinLevel 5

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	ZoneScripts scriptdefs/Deadly_Apocalypse_Hero.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors TH_CH_AbandonedOffice, TH_CH_AbandonedWarehouse, TH_CH_Warehouse, TH_FS_Caves, TH_CH_Sewers, TH_FS_AbandonedOffice, TH_FS_Warehouse, TH_FS_AbandonedWarehouse, TH_FS_Sewers, TH_RR_Caves, TH_GG_Caves, TH_EP_CircleofThorns, TH_EP_Sewers, TH_EH_Caves, TH_EH_CircleofThorns
	VisitLocations Patrol_Hard_5_Hazard_01_02, Patrol_Hard_4_Hazard_01_02, Patrol_Hard_3_Hazard_01_02, Patrol_Hard_2_Hazard_01_02, Patrol_Hard_1_Hazard_01_02, Patrol_Easy_5_Hazard_01_02, Patrol_Easy_4_Hazard_01_02, Patrol_Easy_3_Hazard_01_02, Patrol_Easy_2_Hazard_01_02, Patrol_Easy_1_Hazard_01_02, 
	EncounterLayouts 
	VillainGroups CircleofThorns, Trolls, Outcasts, Igneous, TheLost, Skulls, Hydra, Vahzilok, Tsoo, Coralax, Hellions, LegacyChain
}

// The Tunnels
Map Hazard_01_03.txt
{
	MissionMinLevel 12
	SpawnOverrideMinLevel 12

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Cavern,
	EncounterLayouts Around, Ambush, Phalanx,
	VillainGroups CircleofThorns, Trolls, Igneous, 
}

// Boomtown
Map Hazard_02_01.txt
{
	MissionMinLevel 13
	SpawnOverrideMinLevel 13

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 

//	JP: Believed to be causing lag spikes with insane dbserver -> mapserver broadcasts.
//	ZoneScripts scriptdefs/Holiday_09_Event_Zone_Boomtown.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors AbandonedOffice, Sewers
	VisitLocations Patrol_Easy_5_Hazard_02_01, Patrol_Easy_2_Hazard_02_01, Patrol_Easy_4_Hazard_02_01, Patrol_Hard_3_Hazard_02_01, Patrol_Hard_4_Hazard_02_01, Patrol_Hard_5_Hazard_02_01, Patrol_Hard_1_Hazard_02_01, Patrol_Hard_2_Hazard_02_01, 
	//Plaques WarOnDrugs4, Superadine4, HeroCorps5, HeroCorps3, HeroCorps4, 
	EncounterLayouts Rumble, Single, Phalanx, Around, AroundSpawn, 
	VillainGroups Clockwork, Outcasts, TheLost, Vahzilok, Council,  Trolls, 
}

// Dark Astoria Old
Map Hazard_03_01.txt
{
	MissionMinLevel 23
	SpawnOverrideMinLevel 23

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Gravedigger.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors 
	VisitLocations Patrol_Easy_1_Hazard_03_01, Patrol_Easy_4_Hazard_03_01, Patrol_Easy_3_Hazard_03_01, Patrol_Easy_2_Hazard_03_01, Patrol_Easy_5_Hazard_03_01, Patrol_Hard_2_Hazard_03_01, Patrol_Hard_3_Hazard_03_01, Patrol_Hard_4_Hazard_03_01, Patrol_Hard_5_Hazard_03_01, Patrol_Hard_1_Hazard_03_01, 
	//Plaques SpecialCouncil3, FreedomPhalanx4, SpankyRabinowitz4, SpankyRabinowitz5, 
	EncounterLayouts Rumble, ZombieBurst1, ZombieBurst4, ZombieBurst3, ZombieBurst2, Encounter, Phalanx, Around, Single, 
	VillainGroups Tsoo, BanishedPantheon, CircleofThorns, 
}

// Dark Astoria
Map Coop_06_01.txt
{
	MissionMinLevel 50
	SpawnOverrideMinLevel 50

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/DarkAstoria_Friendly.scriptdef
	ZoneScripts scriptdefs/DarkAstoria_OnEnterGrantPower.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_DarkAstoria.scriptdef
	
	MissionsAllowed All
	PlayersAllowed Hero, Villain
	PersistentNPCs DA_Rogue_CoT, Ephram_Sha, James_Mason, Maharaj, MuVorkan, Midnighter_Loremaster, Midnighter_Archivist, Merit_Vendor_Dark_Astoria_Male, DA_NotorietyContact, TUNNEL_COL,
	MissionDoors AbandonedOffice, AbandonedWarehouse, Caves, Sewers, BPSA5StoryArcDoor, Office, Portal, DA_BP_Caves, DA_DV_Caves, DA_RH_Caves, DA_BP_Sewers, DA_TT_Sewers, DA_DV_Sewers, DA_RH_AbandonedOffice, DA_BP_AbandonedOffice, DA_TT_AbandonedOffice, DA_DV_AbandonedOffice, DA_RA_AbandonedOffice, DA_BP_AbandonedWarehouse, DA_TT_AbandonedWarehouse, DA_RA_Portal1, DA_RA_Portal2, DA_RA_Portal3, DA_RA_Portal4, DA_TT_Portal5, DA_BP_Portal7, A1M1_BP_Caves, A1M2_TT_AbandonedOffice, A1M3_BP_Sewers, A1M4_BP_Sewers, A2M1_TT_Sewers, A2M2_TT_Sewers, A2M3_TT_AbandonedOffice, A2M4_RH_Caves, DA_BP_PSHT, 
	//VisitLocations
	//Plaques
	EncounterLayouts Around_Spread_Out, Phalanx, AroundDoor, Beatup, Rumble, RumbleRanged, Persuade, CenteredAround, Bookie, 
	VillainGroups TsooEndgame, BanishedPantheonEndgame, CircleofThornsEndgame, TalonsOfVengeanceEndgame, KnivesOfVengeanceEndgame, KnivesOfArtemisEndgame, 
}

// Striga Isle
Map Hazard_03_02.txt
{
	MissionMinLevel 20
	SpawnOverrideMinLevel 23

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	
	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors SI_cargoW, SI_cargoC, SI_cargoF, SI_building, SI_warehouse, SI_maw, SI_maw1, SI_mausoleum, SI_vampcave, SI_cave, SI_airbase, SI_lighthouse, SI_base, SI_basebunker, SI_smallrobot1, SI_smallrobot2, SI_giantrobot1, SI_giantrobot2, SI_giantrobot3, SI_radar1, SI_radar2, SI_radar3, SI_radar4, Roy_Mission6
	EncounterLayouts Rumble, Encounter, GraveDig, BigAround, TurkeyShoot, BridgeAssault, StrigaPatrol, GuardShip, CrateMove, Dogfight, Destroy, Duo, ThroatRumble, Snake, Ambush, Phalanx, Around, PlaceBomb, BlockShip, StrigaAirPatrol, StrigaAirPatrolBird, Trio, BrawlShipRec, BrawlShipBox,  
	VillainGroups TheFamily, Warriors, Council, BanishedPantheon, Turrets, SkyRaiders, DevouringEarth
	PersistentNPCs Ravenstorm, Moonfire, Stephanie_Peebles, Long_Jack, Tobias_Hansen, Lars_Hansen, Ernesto_Hess
}

// Crey's Folly
Map Hazard_04_01.txt
{
	MissionMinLevel 32
	SpawnOverrideMinLevel 32

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors AbandonedOffice, AbandonedWarehouse, Warehouse, Sewers, CreyLab
	VisitLocations Patrol_Easy_1_Hazard_04_01, Patrol_Easy_2_Hazard_04_01, Patrol_Easy_3_Hazard_04_01, Patrol_Easy_4_Hazard_04_01, Patrol_Easy_5_Hazard_04_01, Patrol_Hard_1_Hazard_04_01, Patrol_Hard_2_Hazard_04_01, Patrol_Hard_3_Hazard_04_01, Patrol_Hard_4_Hazard_04_01, Patrol_Hard_5_Hazard_04_01, 
	//Plaques RiktiWar2, RiktiWar3, 
	EncounterLayouts Rumble, Trio, Catwalk, Single, Snatch, Ambush, Duo, Encounter, RumbleRanged, Around, Destroy, 
	VillainGroups Crey, Rikti, Freakshow, Nemesis, DevouringEarth, 
}

// Firebase Zulu - Shadow Shard
Map Hazard_06_01.txt
{
	MissionMinLevel 40
	SpawnOverrideMinLevel 40

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_DimensionalExplorer.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Cave_Foxtrot, Cave_XRay, Cave_Tango, Cave_Echo, Tech_Charlie, Tech_Victor, Tech_XRay, Tech_Tango_1, Tech_Sierra, Tech_Alpha, Tech_Foxtrot, Tech_Zulu, Tech_Echo, Tech_Tango_2, SS_Caves, SS_Tech, 
	VisitLocations Hazard_06_01_Device, MonumentEcho1, MonumentSierra1, MonumentSierra2, MonumentSierra3, MonumentSierra4, MonumentTango1, MonumentTango2, MonumentTango3, MonumentTango4, MonumentTango5,
	PersistentNPCs General_Hammond, Dr_Quaterfield, Dr_Boyd, Lt_Volkov, Sgt_Goddard, TUNNEL_SS_PortalCorps,
	EncounterLayouts Rumble, Trio, Encounter, Around, BigAround, Phalanx, Capture, skirmish, Outpost, 
	VillainGroups Crey, Nemesis, Rularuu, Reflections, 
}

// Cascade Archipelago - Shadow Shard
Map Hazard_06_02.txt
{
	MissionMinLevel 40
	SpawnOverrideMinLevel 40

	ZoneScripts scriptdefs/DayJob/DJ_DimensionalExplorer.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Cave_AirShoals, Tech_Waterfall_1, Cave_Chanting_1, Cave_Chanting_2, Cave_Marooned, Tech_Waterfall_2, Cave_OneWay, Cave_Waterfall_3, Cave_Chanting_3, Cave_Tyrant_1, Tech_Marooned, Cave_Tyrant_2, SS_Caves, SS_Tech,
	PersistentNPCs Lt_Col_Flynn, Sara_Moore, Dr_Huxley,
	EncounterLayouts GuardSpot, BigAround, Trio, Battle, Around,
	VillainGroups Nemesis, Rularuu, Reflections, CircleofThorns,
}

// The Chantry - Shadow Shard
Map Hazard_06_03.txt
{
	MissionMinLevel 40
	SpawnOverrideMinLevel 44

	ZoneScripts scriptdefs/DayJob/DJ_DimensionalExplorer.scriptdef

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Cave_Guilt, Cave_Pain, Cave_Sorrow, SS_Caves, SS_Chantry, 
	VisitLocations MonumentPain, MonumentDenial, MonumentGuilt, MonumentShame, MonumentSorrow, MonumentRegret,
	PersistentNPCs Justin_Augustine, Faathim_The_Kind,
	EncounterLayouts BigAround, Trio, Around, Destroy, 
	VillainGroups Rularuu, Reflections, CircleofThorns,
}

// Abandoned Sewer Network
Map Trial_01_01.txt
{
	MissionMinLevel 38
	SpawnOverrideMinLevel 38

	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors SewerTrialRoom
	VisitLocations NemesisSeizesControl3, 
	PersistentNPCs Computer
	EncounterLayouts Ambush, DeathFromAbove, Direction, Around, InWater, 
	VillainGroups CircleOfThorns, Rikti, 
}

// Sewer Network
Map Trial_01_02.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1

	MissionsAllowed Hero
	PlayersAllowed Hero
	EncounterLayouts Ambush, DeathFromAbove, Direction, Around, 
	VillainGroups TheLost, Hellions, Vahzilok, 
}

// Faultline (Old)
Map Trial_02_01.txt
{
	MissionMinLevel 16
	SpawnOverrideMinLevel 16
	
	MissionsAllowed Hero
	PlayersAllowed Hero
	//Plaques WarOnDrugs5, Superadine5, HeroCorps6, HeroCorps7, 
	EncounterLayouts AroundDoor, Single, Around, 
	VillainGroups CircleOfThorns, Clockwork, Vahzilok, 
}

// Terra Volta
Map Trial_03_01.txt
{
	MissionMinLevel 20
	SpawnOverrideMinLevel 25

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
	
	PersistentNPCs JasonS
	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors TV_Reactor
	//Plaques SpecialCouncil4, SpecialCouncil5, FreedomPhalanx5, SpankyRabinowitz6, 
	EncounterLayouts RumbleRanged, Catwalk, AroundDoor, Single, AroundVandalism, RumbleRangedCatwalk, CenteredAround, Phalanx, GroupedDirection, Rumble, RumbleCatwalk, 
	VillainGroups SkyRaiders, TheLost, Freakshow, DevouringEarth, 
}

// Eden
Map Trial_04_01.txt
{
	MissionMinLevel 35
	SpawnOverrideMinLevel 38

	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef 
		
	//Plaques RiktiWar4, RiktiWar5, 
	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors EdenCave, EdenBuilding, EdenTrialRoom
	EncounterLayouts Rumble, Ambush, Phalanx, GroupedDirection, Around, Single, 
	VillainGroups Crey, Nemesis, DevouringEarth, 
}

// The Hive
Map Trial_04_02.txt
{
	EntryMinLevel 45
	MissionMinLevel 45
	MissionsAllowed Hero
	PlayersAllowed Hero
	SpawnOverrideMinLevel 46
	EncounterLayouts Ambush, GroupedDirection, Around, Hamidon, 
	ZoneScripts scriptdefs/Trial_04_02_Hami.scriptdef
}

// Rikti Crash Site (Old)
Map Trial_05_01.txt
{
	MissionMinLevel 40
	SpawnOverrideMinLevel 40
	
	PersistentNPCs Richard_Flagg
	//VisitLocations TV_Patrol_01, TV_Patrol_02, TV_Patrol_03, 
	//Plaques NemesisSeizesControl2, 
	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors Trial_AbandonedOffice
	EncounterLayouts Around_Spread_Out, Outpost, Single, Ambush, Attack, Encounter, skirmish, guard, capture, 
	VillainGroups Crey, Rikti, 
}

// Rikti War Zone
Map War_05_01.txt
{
	MissionMinLevel 35
	SpawnOverrideMinLevel 35
	MissionsAllowed All
	PlayersAllowed Hero, Villain
	ZoneScripts scriptdefs/RiktiWarZone_Friendly.scriptdef
	ZoneScripts scriptdefs/RiktiWarZone_Pylon.scriptdef
	
	PersistentNPCs VanguardDrillInstructor, NotorietyContactRWZ, Notoriety_Contact_RWZ, VanguardQuartermaster_01, VanguardQuartermaster_02, Levantera, Borea, Serpent_Drummer, Gaussian, Dark_Watcher, Lady_Grey, Apex, Tin_Mage, Doctor_Orts
	VisitLocations TV_Patrol_01, TV_Patrol_02, TV_Patrol_03,
	//Plaques NemesisSeizesControl2, RiktiCrashSite1, RiktiWarZone1, RiktiWarZone2, RiktiWarZone3, RiktiWarZone4, RiktiWarZone5, RiktiWarZone6, RiktiWarZone7, RiktiWarZone8, RiktiWarZone9, RiktiWarZone10, RiktiWarZone11, RiktiWarZone12, RiktiWarZone13
	MissionDoors Trial_AbandonedOffice, RWZ_TB_SnakeHole, Vanguard_IOM_Helicopter, RWZ_TB_IOM_Fort, RWZ_P_AbandonedWarehouse, RWZ_TRL_IOM_Fort, RWZ_SR_Cave, RWZ_VC_IOM_BaseInterior, RWZ_PCH_SnakeHole, RWZ_VC_IOM_BaseExterior, RWZ_VB_Portal, RWZ_LRT_SnakeHole, RWZ_BL_AbandonedWarehouse, RWZ_BL_AbandonedTech, RWZ_HR_AbandonedOffice, RWZ_LRT_AbandonedWarehouse, RWZ_SR_Longbow_Cave, RWZ_PDH_SnakeHole, RWZ_PCH_AbandonedTech, RWZ_HR_SnakeHole, RWZ_SR_SnakeHole, RWZ_PDH_IOM_Fort, RWZ_CS_Cave, RWZ_LRT_IOM_Fort, RWZ_TB_AbandonedWarehouse, RWZ_BL_SnakeHole, RWZ_HR_Sewer, Architect
	EncounterLayouts  Trio, Single
	VillainGroups Rikti, Vanguard_Shield, FreedomCorps, Crey, Malta, KnivesofArtemis, Arachnos, Council, DevouringEarth, Nemesis, ParagonPolice,
}

// The Storm Palace - Shadow Shard
Map Trial_06_01.txt
{
	MissionMinLevel 40
	SpawnOverrideMinLevel 46

	ZoneScripts scriptdefs/DayJob/DJ_DimensionalExplorer.scriptdef
	
	MissionsAllowed Hero
	PlayersAllowed Hero
	MissionDoors SS_Caves, SS_StormPalace, Cave_Anger, Cave_Malice, Cave_Torment, Cave_Hatred, Cave_Fury, Cave_Destruction,
	EncounterLayouts Ambush, BigAround, BigGroupedDir, Snake, 
	VisitLocations MonumentMadness, MonumentAnger, MonumentMalice, MonumentTorment, MonumentHatred, MonumentFury, MonumentDestruction,
	VillainGroups Rularuu, StormElementals,
}

// Map Vinceland.txt
// {
// 	// appropriate level ranges for this zone
// 	MissionMinLevel 1
// 	MissionMaxLevel 50
// 	SpawnOverrideMinLevel 1
// 	SpawnOverrideMaxLevel 50
// 
// 	// stuff that exists on this map
// 	MissionsAllowed All
// 	MissionDoors VinceDoor
// //	EncounterLayouts AroundVandalism,
// //	VillainGroups Hellions,
// 
// 	ZoneScripts scriptdefs/PocketD_Teleport.scriptdef
// 	ZoneScripts scriptdefs/Rikti_Invasion_Test.scriptdef
// 	ZoneScripts scriptdefs/Zombie_Apocalypse_Hero.scriptdef
// //	ZoneScripts scriptdefs/Test_CapeBug.scriptdef
// 
// }

// Map Architect.txt
// {
// 	// appropriate level ranges for this zone
// 	MissionMinLevel 1
// 	MissionMaxLevel 50
// 	SpawnOverrideMinLevel 1
// 	SpawnOverrideMaxLevel 50
// 
// 	// stuff that exists on this map
// 	MissionsAllowed All
// 	ZoneScripts scriptdefs/Architect.scriptdef
// 	MissionDoors Architect
// //	EncounterLayouts 
// //	VillainGroups 
// 
// }

// Map Aaronland.txt
// {
// 	// appropriate level ranges for this zone
// 	MissionMinLevel 1
// 	MissionMaxLevel 55
// 	SpawnOverrideMinLevel 1
// 	SpawnOverrideMaxLevel 55
// 
// 	// stuff that exists on this map
// //	MissionDoors AaronDoor
// //	EncounterLayouts AroundVandalism,
// //	VillainGroups Hellions,
// 
// //	ZoneScripts scriptdefs/V_ReclusesVictoryGame.scriptdef
// //	ZoneScripts scriptdefs/V_ReclusesVictoryPvP.scriptdef
// 
// }


//////////////////////////////////// CITY OF VILLAINS ////////////////////////////////////////


// Breakout
Map V_City_00_01.txt
{
	MissionMinLevel 1
	MissionMaxLevel 1
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 1

	ZoneScripts scriptdefs/Tutorial_Level.scriptdef
	
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors VTutotial_Bunker Door
	EncounterLayouts Single, Trio, CenteredAround 
	VillainGroups Zig
	//Plaques Tutorial1
}

// Mercy Island
Map V_City_01_01.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1

	ZoneScripts scriptdefs/LevelOne_LevelUpReminder.scriptdef
	
	PersistentNPCs Mongoose, Kalinda, Arbiter_Diaz, Notoriety_Contact_Mercy, Seer_Marino, Alan_Desslock, Architect_Studio_Manager_V_MI, Architect_Greeter_MI, Architect_Instructor_MI, Architect_ComputerTech_MI, Architect_AsstManager_MI, Architect_MedicalTech_MI, Architect_VenderTech_MI, Longbow_Prisoner, Fire_Wire, Operative_Kuzmin, Doctor_Weber, Lt_Harris, OGTV_Babydoll_Act1, OGTV_Crosscut_Act1, OGTV_DrGraves_Act1, OGTV_Omnicore_Act1,  OGTV_Zephyr_Act1, TUNNEL_Arachnos,
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors MI_Darwin_SHole, MI_Darwin_AbandonedOffice, MI_Darwin_Warehouse, MI_Darwin_Warehouse_Easy, MI_Darwin_Arachnos, MI_Darwin_Manhole, MI_Mercy_SHole, MI_Mercy_Office, MI_Mercy_Warehouse, MI_Mercy_Arachnos, MI_Mercy_Manhole, MI_Mercy_Bank, MI_FTCerberus_SHole, MI_FTCerberus_Arachnos, MI_FTCerberus_Cave, MI_Larry_Casino, MI_FTCerberus_GhostWidow, MI_Darwin_TugBoat, MI_Arachnos_Clocktower, MI_Snakehole_Easy, MI_Darwin_AbandonedOffice, MI_Mercy_Warehouse, MI_FTCerberus_Arachnos, MI_FTCerberus_Sub, MI_Lobby_Arachnos, MI_FTCerberus_AFlyer, MI_Mercy_Truck, Notuseddoor, Architect, MI_Darwin_Tugboat_Easy, MI_Darwin_Arachnos_Easy, MI_Mercy_Truck_Easy, MI_Darwin_Snakehole_Easy, MI_Kuzmin_MissionDoor, MI_DW_M3, HoD_I_1, MI_LH_M4, MI_VEAT_M1
	EncounterLayouts BarFightOut
	VisitLocations 
	VillainGroups Hellions, Skulls, Arachnos, Snakes, Infected, FreedomCorps, RogueIslandPolice, LegacyChain
	//Plaques CoTHistory1, Mercy1, Mercy2, Mercy3, Mercy4, Mercy5, Mercy6
}

// Port Oakes
Map V_City_01_02.txt
{
	MissionMinLevel 5
	SpawnOverrideMinLevel 5

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef
	
	PersistentNPCs Angelo_Vendetti, Billie_Heck, Drea_the_Hook, Mikey_the_Ear, Mr_Bocor, The_Radio, Veluta_Lunata, Brick_Johnson, Architect_ComputerTech, Architect_AsstManager, Architect_MedicalTech, OGTV_Babydoll_Act2, SigArc1M5Contact
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors Office, AbandonedOffice, Caves, CargoShip, Warehouse, AbandonedWarehouse, Sewers, PO_AD_Office, PO_AD_AbandonedOffice, PO_Dock_AbandonedWarehouse, PO_Dock_AbandonedOffice, PO_Dock_Office, PO_CargoShip, PO_OS_Office, PO_M_Office, PO_FH_Caves, CircleofThorns, PO_FH_Office, PO_VM_Office, PO_Bank_01, PO_Bank_02, PO_AD_Caves, PO_Dock_Caves, PO_AD_Sewers, PO_OS_AbandonedWarehouse, PO_OS_Warehouse, PO_Dock_Sewers, PO_OS_Caves, PO_FH_Sewers, PO_M_Sewers, PO_Truck_Easy, PO_Truck_Hard, PO_Casino_01, PO_Casino_02, PO_CargoShip_Sunk, PO_TugBoat, PO_Dock_SnakeHole, PO_Dock_ManHole, PO_Mar_ManHole, PO_Oil_ManHole, PO_FH_Arachnos, PO_VM_Submarine, PO_Lobby_Office, PO_Lobby_Arachnos, PO_Lobby_Arach_Round, Sub, PO_Submarine, Submarine, Architect, VSG1A5M1, VSG1A5M2, VSG1A5M3
	EncounterLayouts Duo, Bookie, Encounter, BackOff, AlleyShootout, Snatch, BarFightOut, Trio, Around, Hustle, StrigaPatrol, Destroy, Barrel, GuardShip, CrateMove, OilBlaze, Caught, Phalanx, GraveDig, 
	VisitLocations 
	VillainGroups Hellions, Arachnos, Council, CircleofThorns, SpectralPirates, TheLost, Coralax, Snakes, 
	//Plaques CoTHistory2, PirateHistory4, PortOakes1, PortOakes2, PortOakes3, PortOakes4
}

// Cap Au Diable
Map V_City_02_01.txt
{
	MissionMinLevel 8
	SpawnOverrideMinLevel 8

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef
	
	PersistentNPCs Dmitri_Krylov, Dr_Shelly_Percey, Golden_Roller, Marshal_Brass, Operative_Wellman, Peter_Themari, Virgil_Tarikoss, Willy_Wheeler, James_Bright, Dean_Yu, Kevin_Gilett, Charles_Ball, Samuel_OReilly, Janitor, Fortunata_Hamilton, Ashley_McKnight, TUNNEL_Arachnos,
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors Office, AbandonedOffice, Caves, CargoShip, Warehouse, AbandonedWarehouse, Sewers, CD_HV_Caves, CD_HV_Arachnos, CD_HV_Sewers, CD_HV_CargoShipA, CD_HV_CargoShipB, CD_HV_Office, CD_CT_Sewers, CD_CT_Warehouse, CD_CT_AbandonedWarehouse, CD_AC_Office, CD_AC_Arachnos, CD_AC_Caves, CD_NH_Office, CD_NH_Sewers, CD_NH_WSPDR, CD_VH_Caves, CD_MD_Caves, CD_MD_Warehouse, Tarikoss_Portal, CD_NH_ManHole, CD_NH_Truck, CD_DCT_ManHole, CD_DCT_Truck, CD_HV_ManHole, CD_HV_Truck, CD_DCT_Tugboat, CD_HV_Tugboat, CD_HV_Casino_01, CD_HV_Casino_02, CD_NH_Bank_01, CAD_Lobby_Office, CAD_Lobby_Arachnos, CD_HV_Sub, Sub, InventionTutorial, Cap_Unique_Midnight, Architect, V_SigArcOneIssue1M1, V_SigArcOneIssue1M2, V_SigArcOneIssue1M3, OGT_Arc3_M1, OGT_Arc3_M2, OGT_Arc3_M4, OGT_Arc3_M5, BaneSpiderRuben_MeetingGrounds
	EncounterLayouts 
	VisitLocations V_Invention_Course_Syllabus, V_Invention_Bookcase, V_Invention_Library_Terminal, V_Invention_Tech_Computer, V_Invention_Storage_Cylinders, V_Invention_Bulletin, V_Invention_Bulletin, V_InventionTutorialWB1, V_InventionTutorialWB2, V_InventionTutorialWB3, V_InventionTutorialWB4
	VillainGroups Vahzilok, Clockwork, Arachnos, Luddites, GoldBrickers, CircleofThorns
	//Plaques DrAeonHistory1, DrAeonHistory2, DrAeonHistory3, CoTHistory3, CapauDiable1, CapauDiable2, CapauDiable3, CapauDiable4, CapauDiable5
	ZoneScripts scriptdefs/V_CapAuDiable_Gremlins.scriptdef 
}

// Sharkhead Isle
Map V_City_03_01.txt
{
	MissionMinLevel 20
	SpawnOverrideMinLevel 20

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/GM-Adamastor.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef
	
	PersistentNPCs Archmage_Tarixus, Captain_Petrovich, Crash_Cage, Doc_Buzzsaw, Lorenz_Ansaldo, Lt_Chalmers, Operative_Kirkland, Operative_Vargas, Vince_Dubrowski, Diviner_Maros, Henri_Dumont, Arbiter_Friesen, Darrin_Wade, Architect_ComputerTech, Architect_AsstManager, Architect_MedicalTech, Silver_Mantis_Sharkhead, Sir_Leonard, Dean_MacArthur, Operative_Luong, Vladimir_Zolner, Lastri_Kayumanis
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors Office, AbandonedOffice, Caves, CargoShip, Warehouse, AbandonedWarehouse, SH_Port_Warehouse, SH_Port_AbandonedWarehouse, SH_Port_Office, SH_Port_AbandonedOffice, SH_Port_CargoShip_Family, SH_Port_CargoShip, SH_Crush_Warehouse, SH_Crush_AbandonedWarehouse, SH_Crush_Caves, SH_Crush_Sewers, SH_Hellforge_Caves, SH_Hellforge_Warehouse, SH_Hellforge_AbandonedWarehouse, SH_Pit_SkyRaiderBase, SH_Pit_Caves, SH_Pit_Council, SH_Pit_Cage_Warehouse, SH_Arachnos, SH_Villa_Office, SH_Villa_AbandonedOffice, SH_Villa_Caves, SH_Villa_SkyRaiderBase, SH_Potters_Mausoleum, SH_Potters_CoT, SH_Potters_Caves, SH_Potters_Caves_Easy, SH_Port_Truck, SH_HForge_Truck, SH_Crush_Truck, SH_Port_ManHole, SH_VR_ManHole, SH_Port_Tugboat, SH_Port_Sub, SH_Casino_O1, SH_Bank_01, Sub, TugBoat, Architect, Vincent_Mission1, Vincent_Mission2, Vincent_Mission3, Vincent_Mission4, Vincent_Mission5, Vincent_Mission6, VSigArcOneIssue2M1, VSigArcOneIssue2M2, VSigArcOneIssue2M3, SSA2M3
	EncounterLayouts 
	VisitLocations 
	VillainGroups Arachnos, CircleofThorns, Council, Freakshow, SkyRaiders, ConsortiumGuards, Scrapyarders, SlagGolems, TheFamily,
	//Plaques CoTHistory4, RecluseHistory2, Sharkhead1, Sharkhead2, Sharkhead3, Sharkhead4
}

// Nerva Archipelago
Map V_City_03_02.txt
{
	MissionMinLevel 24
	SpawnOverrideMinLevel 25

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef
	
	PersistentNPCs TUNNEL_Arachnos,Liutenant_Harriman
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors Office, AbandonedOffice, Caves, CargoShip, Warehouse, AbandonedWarehouse, Sewers, NA_CC_Crey, NA_CC_Longbow, NA_CC_Office, NA_CC_Sewers, NA_CC_Warehouse, NA_A_Longbow, NA_A_Sewers, NA_P_Caves, NA_P_Portal, NA_TI_CircleOfThorns, NA_Casino_01, NA_Bank_01, NA_Bank_02, NA_A_Sub, NA_CC_Tugboat, NA_Lobby_Office, Sub, TugBoat, Architect
	EncounterLayouts Trio, Encounter, Phalanx, Duo, StrigaPatrol, BarFightOut, Destroy, BackOff, GraveDig, GuardShip, BridgeAssault, DogFight
	VisitLocations 
	VillainGroups Coralax, FreedomCorps, TheFamily, TheLost, LegacyChain, Wyvern, Arachnos, Council, Crey, CircleofThorns, DevouringEarth
	//Plaques PirateHistory2, Nerva1, Nerva2, Nerva3, Nerva4, Nerva5
	ZoneScripts scriptdefs/V_Nerva_Demons.scriptdef
}

// St. Martial
Map V_City_04_01.txt
{
	MissionMinLevel 30
	SpawnOverrideMinLevel 30

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef
	ZoneScripts scriptdefs/Lua/StMartial_Family_Event.scriptdef
	
	PersistentNPCs Arbiter_Hayes, Notoriety_Contact_StMartial, Hard_Luck, Hardcase, Basse_Croupier, Jezebel_Jones, Vivacious_Verandi, Johnny_Sonata, Slot_Machine, Gossamer
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors  SM_Bank_01, SM_Bank_02, SM_Bank_03, SM_Casino_01, SM_Casino_02, SM_Lobby_Office, SM_DD_Arachnos, SM_DD_ArachnosFlyer, SM_BM_Caves, SM_B_Sewer, SM_BM_AOffice, SM_Jackpot_Office, SM_THW_ABDWarehouse, SM_THW_Warehouse, SM_B_Sewer, SM_Lobby_Office, SM_THW_Sewers, SM_Flop_Caves, SM_Jackpot_Sewers, SM_Flop_ABDOffice, SM_jackpot_ABDOffice, SM_BM_Warehouse, Office, AbandonedOffice, Caves, Warehouse, AbandonedWarehouse, Sewers,CargoShip,  SM_THW_ABDOffice, SM_BM_ABDWarehouse, SM_TruckDoor, SM_Giza_IntDoor, Architect, GV_BS_Malta, GV_SO_M2, GV_SO_M3, GV_SO_M4, MoleMachine, i24_HiddenBase, i24_SM1M2, i24_SM1M1, i24_SM2M4, i24_SM2M5, i24_SM3M2, i24_SM3M3, i24_SM3M5

	//EncounterLayouts Encounter, Duo, Bookie, GuardShip, Lookout, Phalanx, Abduction, StrigaPatrol, Barrel, Around, OilBlaze, Ambush, Caught, ChaseGuy, BarBrawlOut, BreakOut, CrateMove, PurseSnatch, RangedRumble, PlaceBomb, AroundDoor, AroundVandalism,  
	VisitLocations 
	VillainGroups Arachnos, Carnival, CircleofThorns, DevouringEarth, TheFamily, Freakshow, Tsoo, Wailers,
	//Plaques RecluseHistory1, StMartial1, StMartial2, StMartial3, StMartial4
}

// Grandville
Map V_City_05_01.txt
{
	MissionMinLevel 40
	SpawnOverrideMinLevel 40

	ZoneScripts scriptdefs/Rikti_Invasion.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Villain.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_ArachnosOfficial.scriptdef
	ZoneScripts scriptdefs/NemesisPlot.scriptdef
	
	PersistentNPCs Arbiter_Daos, Dr_Forrester, SigArc1M6Contact, TUNNEL_Arachnos,
	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors Arachnos, Caves, ArachnoidCaves, Sewers, Warehouse, Office, GV_SC_Arachnos, GV_SC_Sewers, GV_SC_Office, GV_TG_Arachnos, GV_TG_Caves, GV_TG_ArachnoidCaves, GV_TG_Sewers, GV_TG_Warehouse, GV_TT_Arachnos, GV_TT_Office, GV_Aeon_Corp, GV_Crey_Corp, GV_SC_Boat, GV_TG_Truck, GV_TG_Warehouse_Low, GV_Arachnoid_Haven, GV_Sub, GV_Arachnos_Flyer, GV_Destiny_Portal, GV_TV_Door, Truck, Sewer, GV_BS_Boat, GV_BS_Sewer, GV_CM_M2, GV_CM_M4, GV_SO_M1, GV_SO_M6

	VisitLocations GVSL8_01, GVSL8_02, GVSL8_03, GVSL8_04, GVSL8_05, GVSL8_06, GVSL8_07, GVSL8_08,GVSL9_01, GVSL9_02, GVSL9_03, GVSL9_04, GVSL9_05, GVSL9_06, GVSL9_07, GVSL9_08, GV_Patron_01, GV_Patron_02, GV_Patron_03, GV_Patron_04
	EncounterLayouts 
	VillainGroups Arachnoids Arachnos Crey KnivesOfArtemis
	//Plaques RecluseHistory4, RecluseHistory5, Grandville1, Grandville2, Grandville3, Grandville4, BlackScorpionPlaque, CaptainMakoPlaque, GhostWidowPlaque, SciroccoPlaque
}

// Ouroboros Villain
Map V_City_06_01.txt
{
	MissionMinLevel 24
	SpawnOverrideMinLevel 25

	ZoneScripts scriptdefs/Ouroboros_Villain_Lvl_Teleport.scriptdef
	ZoneScripts scriptdefs/Ouroboros_Villain_RejectHeroes.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Mender.scriptdef

	MissionsAllowed Villain
	PlayersAllowed Villain
	MissionDoors Portal_Tesseract, Portal_Twilight, Portal_Lazarus, Portal_Pilgrim, Portal_Silos, Portal_Ramiel
	PersistentNPCs V_Mender_Tesseract, V_Mender_Silos, V_Mender_Lazarus, V_Pilgrim, OuroborosStore_01, Mender_Ramiel
	EncounterLayouts 
	VisitLocations 
	VillainGroups 
	//Plaques 
}

// Villain Lounge
Map V_City_06_02.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1

	MissionsAllowed Villain
	PlayersAllowed Villain
//	MissionDoors 
//	PersistentNPCs 
	//Plaques 
}


// Bloody Bay
Map V_PvP_02_01.txt
{
	EntryMinLevel 15
	MissionMinLevel 15
	SpawnOverrideMinLevel 17

	ZoneScripts scriptdefs/PvPLock.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Griefer.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain
	MissionDoors BB_Abandoned_Office, BB_ManHole_Door, BB_Warehouse, BB_CargoShip_Door
	EncounterLayouts  StrigaPatrol, Encounter, TowerTurret, GuardShip, Phalanx, Destroy, GraveDig, BigAround, Bookie, Barrel, Duo 
	VisitLocations BB_VPatrol_01, BB_VPatrol_02, BB_VPatrol_03, BB_VPatrol_04, BB_VPatrol_05, BB_NPatrol_01, BB_NPatrol_02, BB_NPatrol_03, BB_NPatrol_04, BB_NPatrol_05, BB_HPatrol_01, BB_HPatrol_02, BB_HPatrol_03, BB_HPatrol_04, BB_HPatrol_05 
	VillainGroups  Shivan, CircleofThorns, Freakshow, Arachnos, FreedomCorps
	//Plaques PirateHistory1, BloodyBay1, BloodyBay2, BloodyBay3, BloodyBay4
	ZoneScripts scriptdefs/V_BloodyBayPvP.scriptdef
}

// Siren's Call
Map V_PvP_03_01.txt
{
	EntryMinLevel 20
	MissionMinLevel 20
	SpawnOverrideMinLevel 22

	ZoneScripts scriptdefs/PvPLock.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Griefer.scriptdef
	
	MissionsAllowed All
	PlayersAllowed Hero, Villain
	MissionDoors SC_Abandoned_Office, SC_Office, SC_Warehouse_Oil_Rig, SC_ManHole_Door
	EncounterLayouts Trio, Encounter, Phalanx, DogFight, StrigaPatrol, Around, Duo, GuardShip, BarFightOut, Caught, BlockShip, BridgeAssault, BackOff, Apprehend, 
	VisitLocations CS_VPatrol_01, CS_VPatrol_02, CS_VPatrol_03, CS_VPatrol_04, CS_VPatrol_05, CS_NPatrol_01, CS_NPatrol_02, CS_NPatrol_03, CS_NPatrol_04, CS_NPatrol_05, CS_HPatrol_01, CS_HPatrol_02, CS_HPatrol_03, CS_HPatrol_04, CS_HPatrol_05 
	VillainGroups Warriors, Arachnos, FreedomCorps, SkyRaiders, Clockwork, Tsoo
	//Plaques PirateHistory3, SirensCall1, SirensCall2, SirensCall3, SirensCall4
	ZoneScripts scriptdefs/V_SirensCallHunterGame.scriptdef
	ZoneScripts scriptdefs/V_SirensCallPvP.scriptdef
}

// Warburg
Map V_PvP_04_01.txt
{
	EntryMinLevel 30
	MissionMinLevel 30
	SpawnOverrideMinLevel 32

	ZoneScripts scriptdefs/PvPLock.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Griefer.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain
	MissionDoors WB_Abandoned_Office, WB_Manhole_Door
	EncounterLayouts 
	VisitLocations WB_VPatrol_01, WB_VPatrol_02, WB_VPatrol_03, WB_VPatrol_04, WB_VPatrol_05, WB_NPatrol_01, WB_NPatrol_02, WB_NPatrol_03, WB_NPatrol_04, WB_NPatrol_05, WB_HPatrol_01, WB_HPatrol_02, WB_HPatrol_03, WB_HPatrol_04, WB_HPatrol_05
	VillainGroups 
	//Plaques RecluseHistory3, Warburg1, Warburg2, Warburg3
	ZoneScripts scriptdefs/V_WarburgFootballGame.scriptdef
	ZoneScripts scriptdefs/V_WarburgPvP.scriptdef
}

// Recluse's Victory
Map V_PvP_05_01.txt
{
	EntryMinLevel 40
	MissionMinLevel 41
	SpawnOverrideMinLevel 43
	
	ZoneScripts scriptdefs/PvPLock.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Griefer.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain
	MissionDoors 
	EncounterLayouts 
	VisitLocations 
	VillainGroups 
	//Plaques RecluseVictory1, RecluseVictory2, RecluseVictory3, RecluseVictory4
	ZoneScripts scriptdefs/V_ReclusesVictoryGame.scriptdef
	ZoneScripts scriptdefs/V_ReclusesVictoryPvP.scriptdef
}

// Map ejland.txt
// {
// 	ZoneScripts scriptdefs/PDP_Friendly.scriptdef
// 	ZoneScripts scriptdefs/Rikti_Invasion_Test.scriptdef
// }


// The Abyss
Map V_Trial_04_02.txt
{
	EntryMinLevel 45
	MissionMinLevel 45
	MissionsAllowed Villain
	PlayersAllowed Villain
	SpawnOverrideMinLevel 46
	EncounterLayouts Hamidon, 
	ZoneScripts scriptdefs/Trial_04_02_Hami.scriptdef
}

// Monster Island
Map V_Trial_04_03.txt
{
	MissionsAllowed Villain
	PlayersAllowed Villain
	SpawnOverrideMinLevel 46
	EncounterLayouts
}


/////////////////////////////////////////  EVENTS  ///////////////////////////////////////


// Lord Winter's Realm
Map Winter_09_Layout_01.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1
	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
//	ZoneScripts scriptdefs/LordWintersRealm_Friendly.scriptdef
//	ZoneScripts scriptdefs/Holiday_09_Event_Zone_1.scriptdef
}

// Lord Winter's Realm
Map Winter_09_Layout_02.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1
	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
//	ZoneScripts scriptdefs/LordWintersRealm_Friendly.scriptdef
//	ZoneScripts scriptdefs/Holiday_09_Event_Zone_2.scriptdef
}

// Lord Winter's Realm
Map Winter_09_Layout_03.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1
	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
//	ZoneScripts scriptdefs/LordWintersRealm_Friendly.scriptdef
//	ZoneScripts scriptdefs/Holiday_09_Event_Zone_3.scriptdef
}

// Lord Winter's Realm
Map Winter_09_Layout_04.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1
	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
//	ZoneScripts scriptdefs/LordWintersRealm_Friendly.scriptdef
//	ZoneScripts scriptdefs/Holiday_09_Event_Zone_4.scriptdef
}

// Lord Winter's Realm
Map Winter_09_Layout_05.txt
{
	MissionMinLevel 1
	SpawnOverrideMinLevel 1
	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
//	ZoneScripts scriptdefs/LordWintersRealm_Friendly.scriptdef
//	ZoneScripts scriptdefs/Holiday_09_Event_Zone_5.scriptdef
}


/////////////////////////////////////////////////// PRAETORIA //////////////////////////////////////////////


// Praetoria - Nova Praetoria
Map P_City_00_01.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 10
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 10

	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/TheProtest.scriptdef
	ZoneScripts scriptdefs/HamidonTentacle.scriptdef
	ZoneScripts scriptdefs/LevelOne_LevelUpReminder.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Praetoria
	MissionDoors P_NP_GravatechLabs, P_NP_CMF, P_NP_Courthouse, P_NP_NovaholdStorage, P_NP_Syfotine, P_NP_PPDPrecinct5, P_NP_PPDPrecinct7, P_NP_Tunnel_A, P_NP_Tunnel_B, P_NP_Tunnel_C, P_NP_Tunnel_E, P_NP_Tunnel_K, P_NP_Tunnel_L, P_NP_Tunnel_O, P_NP_Tunnel_P, P_NP_Tunnel_S, P_TM_MOI, P_TM_PPDIF, P_TM_PPDWO, P_NP_Office, P_NP_Warehouse, P_NP_Tech, P_NP_Tunnel, P_NP_MA_Tunnel, P_NP_EA_Office, P_NP_EA_Warehouse, P_NP_EA_Tech, P_NP_EA_Tunnel, P_NP_NA_Office, P_NP_NA_Warehouse, P_NP_NA_Tech, P_NP_NA_Tunnel, P_NP_SA_Office, P_NP_SA_Warehouse, P_NP_SA_Tech, P_NP_SA_Tunnel, Office, P_NP_EA_DAI_Tech1, P_NP_EA_DAI_Tech2, P_NP_EA_Reese_Warehouse1, P_NP_SA_Reese_Office1, P_NP_EA_Chief_Office1, P_NP_SA_Chief_Office2, P_NP_EA_Cleopatra_Warehouse1, P_NP_EA_Cleopatra_Tunnel1, P_NP_EA_Flores_Tunnel1, P_NP_SA_Flores_Tunnel2, P_NP_SA_TunnelRat_Tech1, P_NP_SA_TunnelRat_Tech2, P_NP_EA_Arvin_Tunnel1, P_NP_NA_Arvin_Tech1, P_NP_NA_Arvin_Tech2, P_NP_EA_Ricochet_Warehouse1, P_NP_SA_Jackhammer_Tech1, P_NP_SA_Jackhammer_Warehouse1
	PersistentNPCs Dominatrix, Deputy_Assistant_of_Information, Doctor_Arvin, Marauder, Cleopatra, Reese, P_City_00_01_Vendor_56925, P_City_00_01_Vendor_73783, P_City_00_01_Vendor_87878, Washington, P_NP_Zane, P_NP_Seer_0511, P_NP_Specialist_Harrison, P_NP_Flower_Vendor, P_NP_Warrant_Officer_Welks, P_NP_Stern, P_NP_Aria, Warrant, P_NP_Riptide, P_NP_PPD_Van, Robert_Flores, Provost_Marchand, Tunnel_Rat, P_NP_Santiago
	//Plaques PraetoriaTour1, PraetoriaTour2, ColeMemorial, MagisteriumCulture, Statue_Antimatter, Statue_Chimera, Statue_Neuron, Statue_Marauder, Statue_MotherMayhem
	VisitLocations StatueTyrant, StatueDominatrix, StatueMotherMayhem, StatueChimera, StatueNeuron, StatueMarauder
	EncounterLayouts Single
	VillainGroups PraetorianClockworkLoyalist, PraetorianClockworkResistance, PraetorianPolice, PraetorianResistance, TheDestroyers
	GlobalAIBehaviors LeashedToSpawnDistance(210)
}

// Praetoria - Imperial City
Map P_City_00_02.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 20
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 20

	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/HamidonTentacle.scriptdef
	ZoneScripts scriptdefs/TheSyndicate.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Praetoria
	MissionDoors PraetoriaInventionTutorial_R, PraetoriaInventionTutorial_L, P_IC_Resistance, P_IC_CrownDist, P_IC_KantirCorp, P_IC_MICIRF, P_IC_ImperialStorage, P_IC_Omnitek, P_IC_BrandtWare, P_IC_Athena, P_IC_HolonTechnologies, P_IC_PPDPrecinct4, P_IC_VisualDepot, P_IC_VisualCorp, P_IC_Lodestone, P_IC_KearnyLansing, P_IC_TPN, P_IC_BAF, P_IC_Office, P_IC_Tunnel, P_IC_Tech, P_IC_Warehouse, P_IC_LO_Warehouse, P_IC_LO_Tech, P_IC_LO_Office, P_IC_LO_Tunnel, P_IC_PP_Tunnel, P_IC_IA_Warehouse, P_IC_IA_Tech, P_IC_IA_Office, P_IC_IA_Tunnel, P_IC_AU_Warehouse, P_IC_AU_Tech, P_IC_AU_Office, P_IC_AU_Tunnel, P_IC_SG_Warehouse, P_IC_SG_Office, P_IC_SG_Tech, P_IC_SG_Tunnel, Office, P_IC_YinCorp, P_IC_Megalith, P_IC_IA_MrG_Tunnel1, P_IC_IA_MrG_Tech1, P_IC_PP_MrG_Tunnel3, P_IC_IA_Trans_Tech1, P_IC_IA_Trans_Tech2, P_IC_IA_Trans_Tunnel1, P_IC_AU_Baker_Tech1, P_IC_IA_Baker_Tunnel1, P_IC_AU_Chimera_Tunnel1, P_IC_AU_Chimera_Tunnel2, P_IC_AU_Chimera_Office1, P_IC_LO_Chimera_Tech1, P_IC_LO_Chimera_MM1, P_IC_IA_MrG_Tunnel3, P_IC_IA_Kang_Warehouse1, P_IC_LO_Kang_Tech1, P_IC_LO_Kang_Tunnel1, P_IC_AU_Whitworth_Warehouse1, P_IC_AU_McKnight_Tech1, P_IC_AU_MasamotoLab, P_IC_AU_McKnight_Warehouse1, P_IC_AU_McKnight_Office1, P_IC_LO_Flores_Warehouse1, P_IC_PP_Larson_Warehouse1, P_IC_IA_Larson_Warehouse2, P_IC_LO_Larson_Tunnel1, P_IC_PP_Larson_Tunnel2, P_IC_SG_Steffard_Office1, P_IC_IA_Steffard_Tech1, P_IC_SG_1381_Tech1, P_IC_AU_Bobcat_Truck, P_IC_LO_Hatchet_Warehouse1, P_IC_LO_Hatchet_Office1, P_IC_LO_Beholder_Office1, P_IC_AU_Vagabond_Tech1, P_IC_AU_Wardog_Tech1, P_IC_IA_Wardog_Tunnel1, P_IC_SG_Wardog_Tech2, P_IC_IA_Wardog_Tunnel2, P_IC_AU_Bobcat_Office1, P_IC_AU_Bobcat_Warehouse1
	PersistentNPCs Nightstar, P_City_00_02_Vendor_25378, Tailor_Rita_Mayfair, Chimera, Investigator_Whitworth, Interrogator_Kang, Alec_Parson, Mr._G, Tami_Baker, Jessica_Flores, RE_Arcola, Consultant_Rupert, Consultant_Highland, Consultant_Frederick, Consultant_Ross, Consultant_Chesterfield, P_City_00_02_Vendor_53276, P_City_00_02_Vendor_62397, Seer1381, Chance_McKnight, P_IC_Richard_Jones, P_City_00_02_Nurse_Julia, Transmuter, Luke_Larson, Dr_Steffard, P_Bobcat, P_IC_Elana, P_IC_Seer_Resistance, TUNNEL_COL,
	//Plaques PeoplePark, ViewCure, PraetoriaTour3, SevenSpans
	VisitLocations Praetoria_Invention_Center_Curriculum, Praetoria_Invention_Bookcase_L, Praetoria_Invention_Bookcase_R, Praetoria_Invention_Visitor_Console_L, Praetoria_Invention_Visitor_Console_R, Praetoria_Invention_Visualization_Console_L, Praetoria_Invention_Visualization_Console_R, Praetoria_Invention_Visualization_Console_Samples, Praetoria_Invention_Bulletin_L, Praetoria_Invention_Bulletin_R, 
	//EncounterLayouts Single
	VillainGroups PraetorianClockworkLoyalist, PraetorianResistance, PraetorianPolice, PraetorianClockworkResistance, PraetorianSeers, Syndicate, TheDestroyers
	GlobalAIBehaviors LeashedToSpawnDistance(210)
}

// Praetoria - Neutropolis
Map P_City_00_03.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 20
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 20

	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/HamidonTentacle.scriptdef
	ZoneScripts scriptdefs/TheGreatEscape.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Praetoria
	MissionDoors P_NT_NeuronLab, P_NT_NeuronLab2, P_NT_Armatech, P_NT_Hauptman, P_NT_PPDPrecinct11, Office, Tech, Warehouse, Sewer, P_NT_NR_Office, P_NT_Office, P_NT_NR_Tech, P_NT_Tech, P_NT_NR_Warehouse, P_NT_Warehouse, P_NT_LS_Office, P_NT_LS_Tech, P_NT_LS_Warehouse, P_NT_KI_Tech, P_NT_KI_Warehouse, P_NT_Tunnel, P_NT_NR_Tunnel, P_NT_LS_Tunnel, P_NT_KI_Tunnel, P_NT_Luna_Palisade, P_NT_Ivy_Office, P_NT_Ivy_Tunnel, P_NT_Ivy_Tech, P_Ivy_Warehouse, P_NT_Mayhem_Tunnel, P_NT_Mayhem_Office, P_NT_Mayhem_Tunnel2, P_NT_Mayhem_Truck, P_NT_Mayhem_Tunnel3, P_NT_Mayhem_Tunnel4, P_NT_AntiMatter_Tech, P_NT_AntiMatter_Office, P_NT_AM_Tunnel, P_NT_AM_Warehouse, P_AM_TechBio, P_NT_AM_Truck, P_NT_Walker_TechBio, P_NT_Walker_TechBio2, P_NT_WalkerTunnel1, P_NT_Walker_Tunnel2, P_NT_Walker_Truck, P_NT_Walker_Tunnel3, P_NT_Walker_TechBio, P_NT_Yin_Tunnel, P_NT_Yin_TechPower, P_NT_Yin_Office, P_NT_Yin_Warehouse, P_NT_Yin_TechWW, P_Watcher_Tech1, P_NT_Watcher_Tech2, P_NT_Watcher_Tunnel, P_NT_Watcher_Truck, P_NT_LoyEnd_RiftAccess, P_NT_LS_Crow_Tech1, P_NT_LS_Crow_Truck1, P_NT_NR_Crow_Tech2, P_NT_LS_Scott_Warehouse1, P_NT_NR_Hetzfeld_Tunnel1, P_NT_NR_Hetzfeld_Tech1, P_NT_NR_Hetzfeld_Tech2, P_NT_NR_Hetzfeld_Tunnel2, P_NT_LS_Neuron_Tech1, P_NT_NR_Neuron_Tech2, P_NT_NR_Neuron_Warehouse1
	PersistentNPCs Aaron_Walker, Anti-Matter, Dr._Hetzfeld, IVy, Neuron, Battle_Maiden, P_City_00_03_Nurse_Carla, P_City_00_03_Vendor_36287, P_City_00_03_Vendor_48273, P_City_00_03_Vendor_73223, P_NT_Mariana, Administrator_Chen
	//Plaques NeuronTech AntimatterReactor
	//VisitLocations
	//EncounterLayouts Single
	VillainGroups PraetorianClockworkNeuron, PraetorianClockworkLoyalist, PraetorianClockworkAntimatter, PraetorianClockworkResistance, PraetorianPolice, PraetorianResistance, PraetorianSeers, Syndicate, GhoulsDirty
	GlobalAIBehaviors LeashedToSpawnDistance(210)
}

// Praetoria - First Ward
Map P_City_00_04.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 30
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 30

	//ZoneScripts
	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/TheSeedOfHamidon.scriptdef
	ZoneScripts scriptdefs/DayJob/DJ_Scavenger.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Everybody
	MissionDoors P_FW_FFZ_MeredithDoor, P_FW_TaggartSlaverCamp, P_FW_AnnaShepherdHaven, P_FW_MuldoonDUSTCell, P_FW_Arclight, P_FW_BetterMouseTrap, P_FW_Office1381, P_FW_SeerNetwork, P_FW_DSec, P_FW_HauntedApartments, P_FW_TalonsCave, P_FW_Forlorn_Perimeter_Defense, P_FW_SecretMeeting, P_FW_Forlorn_Tunnels_01, P_FW_Overthrow, P_FW_MunitionsDepot, Office, Tunnels, Cave, Warehouse, AbandonedWarehouse, AbandonedOffice, FloodedOffice, P_Tech, P_FW_ET_AbandonedOffice, P_FW_ET_AbandonedWarehouse, P_FW_ET_PTech, P_FW_FFZ_Office, P_FW_FFZ_Warehouse, P_FW_FFZ_AbandonedOffice, P_FW_FFZ_FloodedOffice, P_FW_FFZ_AbandonedWarehouse, P_FW_FFZ_PTech, P_FW_MV_AbandonedOffice, P_FW_MV_AbandonedWarehouse, P_FW_MV_FloodedOffice, P_FW_MV_Caves, P_FW_SC_AbandonedOffice, P_FW_SC_FloodedOffice, P_FW_DUST_Fallback, P_FW_ET_FloodedOffice, P_FW_ET_Tunnels, P_FW_FFZ_Tunnels, P_FW_MV_Tunnels, P_FW_SC_Tunnels, P_FW_FG_Caves
	PersistentNPCs P_AnnaCult_NPC, P_Billystick_NPC, P_Candlestick_NPC, P_CandlestickFreed_NPC, P_CarnivalPrisoner1_NPC, P_CarnivalPrisoner2_NPC, P_Chesire_NPC, P_CompoundGuard_NPC, P_Hetman_NPC, P_Monarch_NPC, P_Gully_NPC, TUNNEL_COL,
	//Plaques
	//VisitLocations
	EncounterLayouts CenteredAround
	VillainGroups Apparitions, ApparitionsPossessed, Awakened, CarnivalOfLight, CarnivalOfVengeance, CarnivalOfWar, DevouringEarthSeed, GhoulsDirty, PraetorianCitizensFirstWard, PraetorianDUST, PraetorianResistance, PraetorianSeers, TalonsOfVengeance
}

// Praetoria - Night Ward
Map P_City_00_09.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 40
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 40

	//ZoneScripts
	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef
	ZoneScripts scriptdefs/Zombie_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/Deadly_Apocalypse_Praetorian.scriptdef
	ZoneScripts scriptdefs/NightWard_OnEnterGrantPower.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Praetoria
	MissionDoors AbandonedOffice, AbandonedWarehouse, Cave, FloodedOffice, Office, P_Tech, Tunnels, Warehouse, P_NW_AQ_AbandonedOffice, P_NW_AQ_AbandonedWarehouse, P_NW_AQ_FloodedOffice, P_NW_AQ_PTech, P_NW_AQ_Tunnels, P_NW_LJ_AbandonedOffice, P_NW_LJ_AbandonedWarehouse, P_NW_LJ_FloodedOffice, P_NW_LJ_Office, P_NW_LJ_PTech, P_NW_LJ_Tunnels, P_NW_LJ_Warehouse, P_NW_TD_AbandonedOffice, P_NW_TD_AbandonedWarehouse, P_NW_TD_Caves, P_NW_TD_FloodedOffice, P_NW_TD_Tunnels, P_NW_CS_AbandonedOffice, P_NW_CS_FloodedOffice, P_NW_CS_Tunnels, P_NW_HV_Caves, P_NW_MM_WestWing, P_NW_MM_EastWing, P_NW_Portal, P_NW_CTA_Building, P_NW_MM_Portal, P_NW_Incarnate_M2, P_NW_Incarnate_M3, P_NW_Incarnate_M4
	PersistentNPCs Merit_Vendor_NightWard_Male_2, P_City_00_09_Store1, P_City_00_09_Store2, Combat_Medic_Female_05, P_Savarius_Trainer, Merit_Vendor_NightWard_Male, P_Montague_Castanella, P_Trilogy, P_Magician, P_Whispering_Coyote, P_Ward, P_Githredeon, P_Sir_Bedwyr, P_Rejuvenate, TUNNEL_COL,
	//Plaques
	//VisitLocations
	EncounterLayouts CenteredAround
	VillainGroups AnimusArcana, Apparitions, BlackKnights, Drudges, Nightstalkers, TalonsOfVengeance
}

// Praetoria - Precinct Five
Map P_City_00_05.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 20
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 20

	ZoneScripts scriptdefs/PraetoriaTutorial.scriptdef
	ZoneScripts scriptdefs/P_Tutorial_Teleport.scriptdef
	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian
	MissionDoors MI_Mercy_SHole
	PersistentNPCs P_Tutorial_Reese, P_Tutorial_Officer_Flint, P_Tutorial_Dr_Steffard, P_Tutorial_Interrogator_Hoffman
	//Plaques 
	//VisitLocations
	//EncounterLayouts Single
	VillainGroups PraetorianClockworkResistance
}

// Praetoria - The Underground (Nova Praetoria)
Map P_City_00_06.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 20
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 20

	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Praetoria
	MissionDoors P_UN_NP_NA_Tunnel, P_UN_NP_EA_Tunnel, Tunnel, P_UN_NP_Arvin_Warehouse1, P_UN_NP_Splice_Tunnel1, P_UN_NP_Jackhammer_Tunnel1, P_UN_NP_Jackhammer_Tunnel2
	PersistentNPCs  P_City_00_06_Vendor_87884, Ricochet, Homeless_Man, Jack_Hammer, Splice
	//Plaques Resistance16, Resistance17, Clean
	//VisitLocations
	//EncounterLayouts Single
	VillainGroups PraetorianResistance, PraetorianPolice
}

// Praetoria - The Underground (Imperial City)
Map P_City_00_07.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 20
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 20

	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Praetoria
	MissionDoors  P_UN_IC_SG_Tunnel, P_UN_IC_LO_Tunnel, Tunnel, P_UN_IC_AU_Tunnel, P_UN_IC_MrG_Tunnel2, P_UN_IC_Hatchet_Tunnel1, P_UN_IC_Kantir, P_UN_IC_Beholder_Tech1, P_UN_IC_Beholder_Tech2, P_UN_IC_Wardog_Warehouse1
	PersistentNPCs  Vagabond, Odysseus, P_City_00_06_Vendor_84445, Clockwork_Tailor_45264, Hatchet, Beholder, Wardog, P_IC_Marty, P_City_00_07_Nurse_Elle, P_City_00_07_Nurse_Mia, TUNNEL_COL,
	//Plaques Resistance18, Resistance19
	//VisitLocations
	//EncounterLayouts Single
	VillainGroups PraetorianResistance, PraetorianPolice
}

// KILL COUNT TEST
// Map IOM_Neutropolis_Tower_i19TF.txt
// {
// 	// appropriate level ranges for this zone
// 	MissionMinLevel 50
// 	MissionMaxLevel 50
// 	SpawnOverrideMinLevel 50
// 	SpawnOverrideMaxLevel 50
// 
// 	MissionsAllowed All
// 	PlayersAllowed Hero, Villain, Praetorian, Primal
// 	TeamArea Praetoria
// 	VillainGroups PraetorianIDF
// }


// Praetoria - The Underground (Neutropolis)
Map P_City_00_08.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 20
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 20

	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Praetorian, Primal
	TeamArea Praetoria
	MissionDoors  P_UN_NT_NR_Tunnel, P_UN_NT_LS_Tunnel, P_UN_NT_LSS_Tunnel, Tunnel, P_UNT_ResEnd_RiftAccess, P_UN_NT_Crow_Tunnel1, P_UN_NT_Crow_Tunnel2, P_UN_NT_Helix_Tunnel1, P_UN_NT_Helix_Tunnel2, P_UN_NT_Helix_Tunnel3, P_UN_NT_Helix_Tunnel4, P_UN_NT_Helix_Tunnel5, P_UN_NT_Scott_Lambda1, P_UN_NT_Scott_Tunnel1, P_UN_NT_Scott_Tech1, P_UN_NT_Scott_Tunnel2
	PersistentNPCs  Crow, Helix, The_Savage, P_NP_Calvin_Scott, P_City_00_06_Vendor_58343, P_City_00_06_Vendor_78263, Belladonna
	//Plaques Resistance20
	//VisitLocations
	//EncounterLayouts Single
	VillainGroups PraetorianResistance, PraetorianPolice
}


// End Praetoria - Behavioral Adjustment Facility
// Map P_City_07_01.txt
// {
// 	EntryMinLevel 50
// 	MissionMinLevel 50
// 	SpawnOverrideMinLevel 50
// 
// 	ZoneScripts scriptdefs/50plusPrison.scriptdef
// 		
// 	//Plaques
// 	//MissionsAllowed Hero
// 	PlayersAllowed Hero, Villain
// 	//MissionDoors PraetoriaUnderground
// 	//EncounterLayouts Rumble, Ambush, Phalanx, GroupedDirection, Around, Single, 
// 	//VillainGroups DevouringEarth, IDF, Resistance
// }

// End Praetoria - Keyes Reactor
// Map P_City_07_02.txt
// {
// 	EntryMinLevel 50
// 	MissionMinLevel 50
// 	SpawnOverrideMinLevel 50
// 
// 	ZoneScripts scriptdefs/50plusKeyesReactor.scriptdef
// 	ZoneScripts scriptdefs/KeyesIsland_CoOp.scriptdef
// 		
// 	//Plaques
// 	//MissionsAllowed Hero
// 	PlayersAllowed Hero, Villain
// 	//MissionDoors PraetoriaUnderground
// 	EncounterLayouts StrigaPatrol, GroupedOutwardLG, ProtectiveCircle, Phalanx, 
// 	//VillainGroups DevouringEarth, IDF, Resistance
// }

// End Praetoria - Lambda Sector
// Map P_City_07_03.txt
// {
// 	EntryMinLevel 50
// 	MissionMinLevel 50
// 	SpawnOverrideMinLevel 50
// 
// 	ZoneScripts scriptdefs/50plusLambdaSector.scriptdef
// 	ZoneScripts scriptdefs/LambdaSector_CoOp.scriptdef
// 		
// 	//Plaques
// 	//MissionsAllowed Hero
// 	PlayersAllowed Hero, Villain
// 	//MissionDoors PraetoriaUnderground
// 	EncounterLayouts Trio, Phalanx, StrigaPatrol,
// 	//VillainGroups DevouringEarth, IDF, Resistance
// }

// End Praetoria - Hospital (Mother of Mercy Psychiatric Hospital)
// Map P_City_07_04.txt
// {
// 	EntryMinLevel 50
// 	MissionMinLevel 50
// 	SpawnOverrideMinLevel 50
// 
// 	ZoneScripts scriptdefs/50plusPrison.scriptdef
// 	ZoneScripts scriptdefs/MotherMercy_CoOp.scriptdef
// 		
// 	//Plaques
// 	//MissionsAllowed Hero
// 	PlayersAllowed Hero
// 	//MissionDoors PraetoriaUnderground
// 	EncounterLayouts CenteredAround, 
// 	//VillainGroups DevouringEarth, IDF, Resistance
// }

// End Praetoria - Prison (Behavioral Adjustment Facility)
// Map P_City_07_05.txt
// {
// 	EntryMinLevel 50
// 	MissionMinLevel 50
// 	SpawnOverrideMinLevel 50
// 
// 	ZoneScripts scriptdefs/50plusHospital.scriptdef 
// 		
// 	//Plaques
// 	//MissionsAllowed Hero
// 	PlayersAllowed Villain
// 	//MissionDoors PraetoriaUnderground
// 	EncounterLayouts CenteredAround, 
// 	//VillainGroups DevouringEarth, IDF, Resistance
// }

// Dave Land test
// Map Dave_Land.txt
// {
// 	EntryMinLevel 50
// 	MissionMinLevel 50
// 	SpawnOverrideMinLevel 50
// 
// 	ZoneScripts scriptdefs/50DaveSector.scriptdef 
// 		
// 	//Plaques
// 	//MissionsAllowed Hero
// 	PlayersAllowed Hero, Villain
// 	//MissionDoors PraetoriaUnderground
// 	EncounterLayouts CenteredAround, 
// 	//VillainGroups DevouringEarth, IDF, Resistance
// }

// Hero Lounge
// Map Hero_Lounge.txt
// {
// 	MissionMinLevel 10
// 	MissionMaxLevel 50
// 	SpawnOverrideMinLevel 10
// 	SpawnOverrideMaxLevel 50
// 
// 	MissionsAllowed All
// 	PlayersAllowed Hero, Villain
// 	ZoneScripts scriptdefs/MidnighterZoneVillain_Teleport.scriptdef
// 
// 	// MissionDoors 
// 	PersistentNPCs Lounge_HeroChatter1 Lounge_HeroChatter2 Lounge_HeroChatter3 Lounge_HeroChatter4 Lounge_HeroChatter5 Lounge_HeroChatter6 Lounge_Botler_VendorWorktable Lounge_Citadel Lounge_Synapse Lounge_Manticore Lounge_Sister_Psyche Lounge_Numina Lounge_Positron Lounge_Statesman Lounge_Botler Lounge_Trashcan_Hero HeroLounge_NurseAlexa_01 Lounge_Trashcan_HeroWorktable James_Frost MsLiberty
// 	// VisitLocations  
// 	// EncounterLayouts 
// 	// VillainGroups
// }

// Villain Lounge
// Map Villain_Lounge.txt
// {
// 	MissionMinLevel 10
// 	MissionMaxLevel 50
// 	SpawnOverrideMinLevel 10
// 	SpawnOverrideMaxLevel 50
// 
// 	MissionsAllowed All
// 	PlayersAllowed Villain
// 	ZoneScripts scriptdefs/MidnighterZoneHeroes_Teleport.scriptdef
// 
// 	// MissionDoors 
// 	PersistentNPCs Lounge_VillainChatter1 Lounge_VillainChatter2 Lounge_VillainChatter3 Lounge_VillainChatter4 Lounge_VillainChatter5 Lounge_VillainChatter6 Lounge_Mistral Lounge_Renault Lounge_Recluse Lounge_Mantis Lounge_Barracuda Lounge_Trashcan_VendorWorktable Lounge_Trashcan VillainLounge_NurseSimone_01 Jacob_Stone Arbiter_Apolis
// 	// VisitLocations  
// 	// EncounterLayouts 
// 	// VillainGroups
// }

// Neutral Tutorial - Destroyed Galaxy City
Map N_City_00_01.txt
{
	// appropriate level ranges for this zone
	MissionMinLevel 1
	MissionMaxLevel 2
	SpawnOverrideMinLevel 1
	SpawnOverrideMaxLevel 2

	ZoneScripts scriptdefs/Praetoria_Friendly_NoPopup.scriptdef
	ZoneScripts scriptdefs/NT_Shivan_Respawns.scriptdef
	ZoneScripts scriptdefs/NT_GiantShivan.scriptdef
	ZoneScripts scriptdefs/NT_AT_Tutorial.scriptdef

	MissionsAllowed All
	PlayersAllowed Hero, Villain, Primal
	PersistentNPCS KO_Hero, Arachnos_Trainer, Longbow_Trainer, OK_Hero, N_Longbow_Pilot, NT_Lord_Recluse, NT_Black_Scorpion, NT_Captain_Mako, NT_Numina, NT_Manticore, NT_Statesman, NT_Lady_Grey, NT_Apex, NT_Dark_Watcher, NT_Scirocco, NT_Ice_Mistral, NT_Aeon, NT_Arachnos_Soldier, NT_Positron
	VillainGroups Shivan, Shivan_Tutorial
	LoadScreenPages intro_comic_cvr, intro_comic_p1, intro_comic_p2, intro_comic_p3, intro_comic_p4
}

// ******************************************************************
// ******************************************************************
// ******************************************************************
// THIS MAP IS COMMENTED OUT BECAUSE THE BEACONIZER IS CRASHING ON IT!
// TALK TO GRAHAM BEFORE UNCOMMENTING IT, PLEASE!
// ******************************************************************
// ******************************************************************
// ******************************************************************
// End Praetoria - Magisterium
//Map P_City_07_06.txt
//{
//	EntryMinLevel 50
//	MissionMinLevel 50
//	SpawnOverrideMinLevel 50

//	ZoneScripts scriptdefs/50plusMagisterium.scriptdef 
		
	//Plaques
	//MissionsAllowed Hero
//	PlayersAllowed Hero, Villain
	//MissionDoors PraetoriaUnderground
//	EncounterLayouts Rumble, Ambush, Phalanx, GroupedDirection, Around, Single, 
	//VillainGroups DevouringEarth, IDF, Resistance
//}

// HeroCon Demo Map
Map Events_HeroCon.txt
{
	EntryMinLevel 1
	MissionMinLevel 50
	SpawnOverrideMinLevel 50
		
	//Plaques
	//MissionsAllowed Hero
	PlayersAllowed Hero, Villain
	//MissionDoors 
	//VillainGroups PraetorianClockworkLoyalist
}




