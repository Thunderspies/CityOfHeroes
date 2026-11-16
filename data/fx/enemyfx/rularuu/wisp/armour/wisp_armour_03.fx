#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################

############## RIGHT ARM

Condition
	On 	Time
	Time 	0	

	Event
		EName 	Wisp_WristRight_01
		Type	local
		At	lArmR
		Geom	GEO_Wisp_Armour_01
		Bhvr	:Wisp_RightWrist_01.bhvr
		sound	blue2_loop 60 50 .35
	End	

	Event
		EName 	Wisp_Armour_01
		Type	local
		At	UArmR
		Geom	GEO_Wisp_Armour_01
		Bhvr	:Wisp_RightArm_01.bhvr
		sound	purple_loop 60 50 .35
	End	
	Event
		EName 	Wisp_Armour_02
		Type	local
		At	UArmR
		Geom	GEO_Wisp_Armour_02
		Bhvr	:Wisp_RightArm_02.bhvr
	End

	Event
		EName 	Wisp_Armour_03
		Type	local
		At	LArmR
		Bhvr	:Wisp_RightArm_03.bhvr
	End

	Event
		EName 	Wisp_Armour_04
		Type	local
		At	Wisp_Armour_03
		Geom	GEO_Wisp_Armour_03
		Bhvr	:Wisp_RightArm_03a.bhvr
	End

	Event
		EName 	Wisp_Armour_05
		Type	local
		At	Wisp_Armour_04
		Geom	GEO_Wisp_Armour_04
		Bhvr	:Wisp_RightArm_03b.bhvr
	End

	Event
		EName 	Wisp_Shoulder_01
		Type	local
		At	Col_R
		Bhvr	:Wisp_RightArm_04.bhvr
	End

	Event
		EName 	Wisp_Shoulder_02
		Type	local
		At	Wisp_Shoulder_01
		Geom	GEO_Wisp_Armour_05
		Bhvr	:Wisp_RightArm_04a.bhvr
	End

	Event
		EName 	Wisp_Shoulder_03
		Type	local
		At	Wisp_Shoulder_01
		Geom	GEO_Wisp_Armour_06
		Bhvr	:Wisp_RightArm_04b.bhvr
	End

############## RIGHT ARM GLOWS

	Event
		EName 	Wisp_LeftArmGlow_01
		Type	local
		At	LArmR
		Part	:Wisp_GlowRing_01.Part
		Bhvr	:Wisp_GlowRing_01.bhvr
	End

	Event
		EName 	Wisp_LeftArmGlow_02
		Type	local
		At	LArmR
		Part	:Wisp_Glow_01.Part
		Part	:Wisp_Glow_02.Part
		Part	:Wisp_Glow_Big.Part
		Bhvr	:Wisp_Glow_01.bhvr
	End

	Event
		EName 	Wisp_LeftArmGlow_01
		Type	local
		At	UArmR
		Part	:Wisp_GlowRing_01.Part
		Bhvr	:Wisp_GlowRing_02.bhvr
	End

	Event
		EName 	Wisp_LeftArmGlow_02
		Type	local
		At	UArmR
		Part	:Wisp_Glow_01.Part
		Part	:Wisp_Glow_02.Part
		Part	:Wisp_Glow_Big.Part
		Bhvr	:Wisp_Glow_01.bhvr
	End

############## LEFT ARM

	Event
		EName 	Wisp_LeftArm_01
		Type	local
		At	UArmL
		Geom	GEO_Wisp_Armour_01
		Bhvr	:Wisp_LeftArm_01.bhvr
	End	

	Event
		EName 	Wisp_LeftArm_02
		Type	local
		At	UArmL
		Geom	GEO_Wisp_Armour_02
		Bhvr	:Wisp_LeftArm_02.bhvr
	End

	Event
		EName 	Wisp_Arm_01
		Type	local
		At	UArmL
		Bhvr	:Wisp_LeftArm_03.bhvr
	End

	Event
		EName 	Wisp_Arm_02
		Type	local
		At	Wisp_Arm_01
		Geom	GEO_Wisp_Armour_03
		Bhvr	:Wisp_LeftArm_03a.bhvr
	End

	Event
		EName 	Wisp_Arm_03
		Type	local
		At	Wisp_Arm_01
		Geom	GEO_Wisp_Armour_04
		Bhvr	:Wisp_LeftArm_03b.bhvr
	End

	Event
		EName 	Wisp_Wrist_01
		Type	local
		At	LArmL
		Bhvr	:Wisp_LeftArm_03.bhvr
	End

	Event
		EName 	Wisp_Wrist_02
		Type	local
		At	Wisp_Wrist_01
		Geom	GEO_Wisp_Armour_06
		Bhvr	:Wisp_LeftArm_03c.bhvr
	End

	Event
		EName 	Wisp_Wrist_03
		Type	local
		At	LArmL
		Geom	GEO_Wisp_Armour_02
		Bhvr	:Wisp_LeftArm_03d.bhvr
	End

############## LEFT ARM GLOWS

	Event
		EName 	Wisp_LeftArmGlow_01
		Type	local
		At	LArmL
		Part	:Wisp_GlowRingLeft_01.Part
		Bhvr	:Wisp_GlowRingLeft_01.bhvr
	End

	Event
		EName 	Wisp_LeftArmGlow_02
		Type	local
		At	LArmL
		Part	:Wisp_Glow_01.Part
		Part	:Wisp_Glow_02.Part
		Part	:Wisp_Glow_Big.Part
		Bhvr	:Wisp_GlowLeft_01.bhvr
	End

	Event
		EName 	Wisp_LeftArmGlow_01
		Type	local
		At	UArmL
		Part	:Wisp_GlowRing_01.Part
		Bhvr	:Wisp_GlowRingLeft_02.bhvr
	End

	Event
		EName 	Wisp_LeftArmGlow_02
		Type	local
		At	UArmL
		Part	:Wisp_Glow_01.Part
		Part	:Wisp_Glow_02.Part
		Part	:Wisp_Glow_Big.Part
		Bhvr	:Wisp_GlowLeft_01.bhvr
	End

############### SHOULDER GLOW

	Event
		EName 	Wisp_LeftArmGlow_02
		Type	local
		At	Col_R
		Part	:Wisp_Glow_Big.Part
		Bhvr	:Wisp_Glow_Shoulder.bhvr
	End

############## LEG RIGHT 
	Event
		EName 	Wisp_RightLegRing_01
		Type	local
		At	ULegR
		Part	:Wisp_GlowRing_01.Part
		Bhvr	:Wisp_GlowRingLegRight_01.bhvr
	End

	Event
		EName 	Wisp_RightLegGlow_01
		Type	local
		At	ULegR
		Part	:Wisp_Glow_01.Part
		Part	:Wisp_Glow_02.Part
		Part	:Wisp_Glow_Big.Part
		Bhvr	:Wisp_GlowRingLegRight_03.bhvr
	End

	Event
		EName 	Wisp_RightLegArmour_01
		Type	local
		At	ULegR
		Geom	GEO_Wisp_Armour_03
		Bhvr	:Wisp_GlowRingLegRight_02.bhvr
	End

###############	BODY GLOW
Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:HealingFlamesFeet.part
		PMagnet Hips

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:LegHealingFlames2.part
		PMagnet ULEGL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:HealingFlamesFeet.part
		PMagnet LlegL
	End



#LegR



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:HealingFlamesFeet.part
		PMagnet Hips

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:LegHealingFlames2.part
		PMagnet ULegR
	End


	Event
		EName	2
		Type	Local
		At	FootR
		part1	:HealingFlamesFeet.part
		PMagnet LlegR
	End
		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part2	:HealingFlames.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:HealingFlames.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:HealingFlames.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:HealingFlames.part
		PMagnet	LArmL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:HealingFlames.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:HealingFlames.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:HealingFlames.part
		PMagnet LArmR
	End

	Event
		EName	15
		Type	Local
		At	Chest
		part1	:HealingFlames.part
	End

	Event
		EName	16
		Type	Local
		At	Hair
		part1	:HealingFlames.part
	End

End
