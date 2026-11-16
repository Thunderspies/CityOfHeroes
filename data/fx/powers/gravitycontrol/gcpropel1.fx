#############################################################
## GCPropel1.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prop_rise
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 6.0
		End
		Anim	Prop_basic
		Sound thunder2 100 100 .66
	End

	Event
		Ename	LiftForce
		Type	Posit
		At	Prop_rise
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		12
			PhysForcePower		30
			PhysForcePowerJitter	5
		End
		lifeSpan 90
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Prop_rise
		BhvrOverride
			PositionOffset		0 3 0
			PhysForceType		In
			PhysForceRadius		15
			PhysForcePower		25
			PhysForcePowerJitter	5
		End
		lifeSpan		90
	End
End

Condition
	On	Time
	Time	11

	Event
		EName	Prop_chunk
		Type	Local
		At	Prop_rise
		AnimPiv Hips
	End

	Event
		EName	Prop_chunk_node
		Type	Posit
		At	Prop_chunk
		Sound gravityzap4 100 100 .8
	End
End

Condition
	On	Time
	Time	23

	Event
		EName	Prop_chunk_node2
		Type	Local
		BhvrOverride
			InitialVelocity	0.0 0.07 0.0
		End
		At	Prop_chunk
		Geom	Prop_BoomBox
		Geom	Prop_box_recruit
		Geom	Prop_Jackhammer
		Geom	Computer_Monitor_01
		Geom	Computer_PC_01
		Geom	barstool
		Geom	bartable
		Geom	barchair
		Geom	Chair_Generic_Wait_02
		Geom	Med_Wait_Table
		Geom	Chair_Leather_Exec_01
	#	Geom	Chair_Executive_Wait_01 <<no longer has texture
		Geom	Couch_Exec_01
		Geom	bartaps
		Geom	JukeBox
		Geom	PoolTable
		Geom	Speaker_01
		Geom	TV_Generic_Med
		Geom	TV_Generic_Sm
		Geom	Tv_PlasmaFlat_lg
		Geom	Med_Air_Tank_02
		Geom	Med_Air_Tank_03
		Geom	FileCabinet_Lg_01
		Geom	FileCabinet_Sm_01
		Geom	Wrhs_Crate_01
		Geom	Wrhs_Crate_02
		Geom	Wrhs_Crate_03
		Geom	Wrhs_Crate_04
		Geom	Wrhs_Crate_05
		Geom	Aircompsr_Generic
		Geom	Forklft_Generic
		Geom	Chem_55gal_01
		Geom	JunkBarrel_55gal_01
		Geom	JunkBarrel_55gal_02
		Geom	CoT_Barrel_01
		Geom	Tek1_Barrel1
		Geom	Tek1_Canister_sml_02
		Geom	FifthClmn_Barrel1
		Geom	Arac_Barrel_01
		Geom	OilDrum
		Geom	Junk_Burnt_Couch
		Geom	Junk_Car_01
		Geom	Junk_refridgerator
		Geom	Junk_shoppingcart
		Geom	Pallet_plain
		Geom	OO_Floor_Safe_01
		Geom	OO_Treasurechest_01
		Geom	OO_Bomb_Lrg_01
		Geom	OO_Refrigerator_01
		Geom	OO_Coffin_01
		Geom	_OO_Relic_Box_01
		Geom	Desk_Exec_01
		Geom	Desk_Plain_01
		Geom	Desk_Exec_02
		Geom	Table_Confnc_02_Sm
		Geom	Lamp_Tabl_Antique_03
		Geom	Lamp_Tabl_Tech_02
		Geom	Lamp_Tabl_Tech_01
		Geom	Lamp_Floor_Tech_01
		Geom	Lamp_Floor_Antique_01
		Geom	Medical_Rolling_Stand
		Geom	Med_Computer_01
		Geom	Med_Computer_02
		Geom	Med_Computer_04
		Geom	Med_Computer_05
		Geom	Med_Box_Closed
		Geom	Med_Gurney_01
		Geom	Medical_Tool_Stand
		Geom	PhotoCopier
		Geom	DrinkingFountain
		Geom	TrashBin_Recycl
		Geom	TrashBin_Plain
		Geom	TrashCan_Sm
		Geom	AshTray_01
		Geom	Water_Cooler
		Geom	Junk_Burnt_Mattress
		Geom	Junk_Scrap_014
		Geom	Junk_Scrap_05
		Geom	_dmpstr_sm
		Geom	_dmpstr_closed
		Geom	wrhscase_A
		Geom	Generator_Sleek
		Geom	Generator_Military
		Geom	Handcart_2whl
		Geom	Handcart_4whl
		Geom	Transformer2
		Geom	Transformer1
		Geom	wrhs_insul_roll_A
		Geom	wrhs_crate_pal_A
		Geom	Crate_sm_Wood_01
		Geom	Wirespool_full01
		Geom	Wirespool_empty01
		Geom	pwrbox_sml_01
		Geom	pwrbox_sml_02
		Geom	_Shop_DispBox_Lg_Main
		Geom	Shop_Shelf_steel
		Geom	Shop_Counter_Corner
		Geom	_str_wast_02
		Geom	_roo_wgar_test01
		Geom	_roo_ugar_03
		Geom	_roo_wgar_test02
		Geom	_roo_ugar_02b
		Geom	_roo_ugar_06b
		Geom	_roo_ugar_04b
		Geom	_roo_ugar_07b
		Geom	_twn_stat_01
		Geom	_twn_stat_02
		Geom	_twn_stat_04
		Geom	_twn_stat_05
		Geom	_twn_stat_07
		Geom	_twn_stat_10
		Geom	_twn_stat_11
		Geom	_Univ_InvStation_lo
		Geom	Univ_WallShort04
		Geom	Univ_Rail08
		Geom	Univ_Light_Hanging01
		Geom	Univ_Bookcase04_Single
		Geom	Univ_Table02
		Geom	Univ_Chair01
		Geom	Univ_Table01
		Geom	Univ_Light_TableLamp01
		Geom	prn_toilet_01
		Geom	prn_sink_01c
		Geom	prn_sink_01b
		Geom	prn_sink_01d
		Geom	prn_toilet_01b
		Geom	Fx_BathroomStallDoor
		Geom	FX_StreetLight_01
		Geom	Fx_Streetlight_02
		Geom	FX_gaslamp_01
		Geom	_telepole_Broke1
		Bhvr	behaviors/GCPropelScale.bhvr
		Part	:GCRockGlow.part
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Prop_chunk_FX
		Type	Start
		At	Prop_chunk_node
		  #AnimPiv Hips
		Part	:GCVortexGround.part
		Part	:GCGlowGroundSmall.part
		Part	:GCVortexGlowsUp2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	50

	Event
		EName	Prop_chunk
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node2
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node
		Type	Destroy
	End

	Event
		EName 	Prime
		Type	Start
		At	Prop_chunk_node2
		Bhvr	behaviors\GCPropel.bhvr
		BhvrOverride
			TrackRate		5.0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	10
		End
		Geom	Parent
		Sound Rockthrow 100 100 .9
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	PushDirection
		Type	Posit
		At	Prop_rise
		Lookat	Target
		LifeSpan 20
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 2.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		15
			PhysForcePower		750
			PhysForcePowerJitter	100
		End
		LifeSpan 3
	End
End

#########################################################
#################### CHUNK EFFECTS ######################
#########################################################

Condition
	On	Time
	Time	50

	Event
		EName	Ring_node
		Type	Local
		At	Prime
		Geom	FrontFacingLocalPivot
		#Lookat	Target
	End

	Event
		ENAME	ChunkRings
		Type	Posit
		At	Ring_node
		AltPiv	1
		part1	:GCChunkRings.part
	End

	Event
		ENAME	RockStreaks
		Type	Posit
		At	Ring_node
		part	:GCRockStreaks1.part
		Lifespan 60

	End
End

Condition
	On	Primehit

	Event
		EName	Prop_rise
		Type	Destroy
	End

	Event
		EName	Prop_chunk_FX
		Type	Destroy
	End

	Event
		EName	Ring_node
		Type	Destroy
	End

	Event
		EName	ChunkRings
		Type	Destroy
	End

	Event
		EName	RockStreaks
		Type	Destroy
	End

	Event
		EName	ChestGlow
		Type	Destroy
	End

	Event
		EName	HeadGlow
		Type	Destroy
	End

	Event
		EName	LeftUarmGlow
		Type	Destroy
	End

	Event
		EName	LeftLarmGlow
		Type	Destroy
	End

	Event
		EName	NeckNode
		Type	Destroy
	End

	Event
		EName	ChestNode
		Type	Destroy
	End

	Event
		EName	LarmLNode
		Type	Destroy
	End

	Event
		PhysicsOnly
		EName 	Bounce
		Type	Start
		At	Prime
		Geom	Parent
		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PhysDebris	1
		End
		Lifespan	30
	End

	Event
		EName	ChestGlow2
		Type	Destroy
	End

	Event
		EName	HeadGlow2
		Type	Destroy
	End

	Event
		EName	LeftUarmGlow2
		Type	Destroy
	End

	Event
		EName	LeftLarmGlow2
		Type	Destroy
	End

	Event
		EName	NeckNode2
		Type	Destroy
	End

	Event
		EName	ChestNode2
		Type	Destroy
	End

	Event
		EName	LarmLNode2
		Type	Destroy
	End

	Event
		EName	Prime
		Type	Destroy
	End

End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .88
	End
End

Condition
	On	Time
	Time	5

	Event
		ENAME	NeckNode
		Type	Local
		At	Neck

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet NeckNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	ChestNode
		Type	Local
		At	Chest

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlow.part
		Pmagnet ChestNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

End

#########################################################
################## BODY GLOW MIST #######################
#########################################################



Condition
	On	Time
	Time	0

	Event
		ENAME	NeckNode2
		Type	Local
		At	Neck

	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part1	:GCBodyGlowMist.part
		Pmagnet NeckNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	ChestNode2
		Type	Local
		At	Chest

	End

	Event
		ENAME	HeadGlow2
		Type	Local
		At	Hair
		part1	:GCBodyGlowMist.part
		Pmagnet ChestNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LarmLNode2
		Type	Local
		At	LarmL

	End

	Event
		ENAME	LeftUarmGlow2
		Type	Local
		At	SpadL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow2
		Type	Local
		At	WepL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

End


End
##################################

