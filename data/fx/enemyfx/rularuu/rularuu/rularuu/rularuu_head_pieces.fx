#########################################################
##	Template

FxInfo


Condition
	On 	Time
	Time 	0

#######  OFFSETS
	Event
		EName	Offset_Spin_01
		Type	Local
		At	Hair
		Bhvr	:Offset_Spin_01.bhvr

	End
	Event
		EName	Offset_Spin_02
		Type	Local
		At	Hair
		Bhvr	:Offset_Spin_02.bhvr

	End

######## GEOMETRY

	Event
		EName	Offset
		Type	Local
		At	Offset_Spin_01
		Geom	Rularuu_Head_05
	End

	Event
		EName	Offset
		Type	Local
		At	Offset_Spin_02
		Geom	Rularuu_Head_04
	End

	Event
		EName	Offset
		Type	Local
		At	Hair
		Geom	Rularuu_Head_01
		Bhvr	:Offset_01.bhvr
	End

	Event
		EName	Offset
		Type	Local
		At	Hair
		Geom	Rularuu_Head_02
		Bhvr	:Offset_02.bhvr
	End

	Event
		EName	Offset
		Type	Local
		At	Hair
		Geom	Rularuu_Head_02
		Bhvr	:Offset_03.bhvr
	End
	
	Event
		EName	Offset
		Type	Local
		At	Hair
		Geom	Rularuu_Head_01
		Bhvr	:Offset_04.bhvr
	End

	Event
		EName	Offset
		Type	Local
		At	Hair
		Geom	Rularuu_Head_03
		Bhvr	:Offset_05.bhvr
	End

########## FX  HEAD
	
	Event
		EName	FX
		Type	Local
		At	Hair
		Part	:Headglow_Main.part
		Bhvr	:Offset_Headglow.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	Hair
		Part	:Headglow_Specs.part
	End

	Event
		EName	FX
		Type	Local
		At	Head
		Part	:Headglow_Inner.part
		Bhvr	:Offset_Headglow_inner.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	Head
		Part	:Eye_Right_Main.part
		Part	:Eye_Right_Gas.part
		Bhvr	:Offset_Eye_Right.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	Head
		Part	:Eye_Right_Main.part
		Part	:Eye_Left_Gas.part
		Bhvr	:Offset_Eye_Left.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	Head
		Part	:Crown_Flat.part
		Bhvr	:Offset_Crown_Base.bhvr
	End

	Event
		EName	Offset_Crown_Glow_Spin
		Type	Local
		At	Head
		Bhvr	:Offset_Crown_Glow_Spin.bhvr
	End

#	Event
#		EName	Offset_Crown_Glow_Spin
#		Type	Local
#		At	Head
#		Bhvr	:Offset_Crown_Glow_Spin.bhvr
#	End
#
#	Event
#		EName	Offset_Crown_Glow_Outpost
#		Type	Local
#		At	Offset_Crown_Glow_Spin
#		Bhvr	:Offset_Crown_Glow_Outpost.bhvr
#	End
#
#	Event
#		EName	Offset_Crown_Glow_Outpost_Spin
#		Type	Local
#		At	Offset_Crown_Glow_Outpost
#		Bhvr	:Offset_Crown_Glow_Outpost_Spin.bhvr
#	End
#
#	Event
#		EName	FX
#		Type	Local
#		At	Offset_Crown_Glow_Outpost_Spin
#		Part	:Crown_Glow_01.part
#	End

########## FX  CHEST

	Event
		EName	FX
		Type	Local
		At	Chest
		Part	:ChestGlow_Main.part
		Bhvr	:Offset_ChestGlow.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	Chest
		Part	:ChestGlow_Specs.part
		Bhvr	:Offset_ChestGlow.bhvr
	End

	Event
		EName	OffsetChestGlowNeck
		Type	Local
		At	Chest
		Bhvr	:Offset_ChestGlowNeck.bhvr
	End

	Event
		EName	OffsetChestGlowChest1
		Type	Local
		At	Chest
		Bhvr	:Offset_ChestGlowChest1.bhvr
	End

	Event
		EName	OffsetChestGlowHips
		Type	Local
		At	Chest
		Bhvr	:Offset_ChestGlowHips.bhvr
	End

	Event
		EName	OffsetChestGlowChest2
		Type	Local
		At	Chest
		Bhvr	:Offset_ChestGlowChest2.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	OffsetChestGlowChest1
		Part	:ChestGlow_Inner.part
		POther	OffsetChestGlowHips
	End

	Event
		EName	FX
		Type	Local
		At	OffsetChestGlowHips
		Part	:ChestGlow_Inner.part
		POther	OffsetChestGlowChest2
	End

	Event
		EName	FX
		Type	Local
		At	Col_R
		Part	:ChestGlow_Inner.part
		POther	OffsetChestGlowChest2
	End

	Event
		EName	FX
		Type	Local
		At	OffsetChestGlowNeck
		Part	:ChestGlow_Inner.part
		POther	OffsetChestGlowChest1
	End

########## FX  ARM INNER GLOW RIGHT

	Event
		EName	FX
		Type	Local
		At	UArmR
		Part	:ArmGlow_Shoulder.part
		Part	:ArmGlow_Right_Shoulder_Specs.part
		Bhvr	:Offset_ArmGlow_Shoulder_Right.bhvr
	End

	Event
		EName	Offset_ArmGlow_LArm_Right
		Type	Local
		At	LArmR
		Bhvr	:Offset_ArmGlow_LArm_Right.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	UArmR
		Part	:ArmGlow_UArmR.part
		Part	:ArmGlow_Right_Specs.part
		Bhvr	:Offset_ArmGlow_Shoulder_Right.bhvr
		POther	Offset_ArmGlow_LArm_Right
	End

	Event
		EName	FX
		Type	Local
		At	LArmR
		Part	:ArmGlow_LArmR.part
		Part	:ArmGlow_LArm_Right_Specs.part
		Bhvr	:Offset_ArmGlow_LArm_Right2.bhvr
	End

########## FX  ARM INNER GLOW Left

	Event
		EName	FX
		Type	Local
		At	UArmL
		Part	:ArmGlow_Shoulder.part
		Part	:ArmGlow_Right_Shoulder_Specs.part
		Bhvr	:Offset_ArmGlow_Shoulder_Right.bhvr
	End

	Event
		EName	Offset_ArmGlow_LArm_Left
		Type	Local
		At	LArmL
		Bhvr	:Offset_ArmGlow_LArm_Left.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	UArmL
		Part	:ArmGlow_UArmL.part
		Part	:ArmGlow_Left_Specs.part
		Bhvr	:Offset_ArmGlow_Shoulder_Right.bhvr
		POther	Offset_ArmGlow_LArm_Left
	End

	Event
		EName	FX
		Type	Local
		At	LArmL
		Part	:ArmGlow_LArmR.part
		Part	:ArmGlow_LArm_Right_Specs.part
		Bhvr	:Offset_ArmGlow_LArm_Left2.bhvr
	End

	Event
		EName	FX
		Type	Local
		At	LArmL
		Part	:ArmGlow_LArmL.part
	End

########## FX  ARMBEAMS

	Event
		EName	Offset
		Type	Posit
		At	HandR
		Part	:ArmBeam_LArm.part
	End

	Event
		EName	Offset
		Type	Posit
		At	LArmR
		Part	:ArmBeam_LArm.part
	End

	Event
		EName	Offset
		Type	Posit
		At	UArmR
		Part	:ArmBeam_LArm.part
	End

	Event
		EName	Offset
		Type	Posit
		At	HandL
		Part	:ArmBeam_LArm.part
	End

	Event
		EName	Offset
		Type	Posit
		At	LArmL
		Part	:ArmBeam_LArm.part
	End

	Event
		EName	Offset
		Type	Posit
		At	UArmL
		Part	:ArmBeam_LArm.part
	End

	Event
		EName	Offset
		Type	Posit
		At	Neck
		Part	:ArmBeam_LArm.part
	End

End
