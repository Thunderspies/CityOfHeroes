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
		EName 	Wisp_Armour_01
		Type	local
		At	UArmR
		Geom	GEO_Wisp_Armour_01
		Bhvr	:Wisp_RightArm_01.bhvr
		sound	red_loop 60 50 .5

	End	
	Event
		EName 	Wisp_Armour_02
		Type	local
		At	UArmR
		Geom	GEO_Wisp_Armour_02
		Bhvr	:Wisp_RightArm_02.bhvr
		sound	rune_loop 60 50 .88
	End


############## RIGHT ARM GLOWS

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