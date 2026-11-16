#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################

############## RIGHT ARM

Condition
	On 	Time
	Time 	0	

	

###############	BODY GLOW

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		PMagnet Hips
		sound	red_loop 60 50 .4

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther ULEGL
		sound	blue2_loop 60 50 .35
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther LlegL
	End



#LegR



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther Hips

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther ULegR
	End


	Event
		EName	2
		Type	Local
		At	FootR
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther LlegR
	End
		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Col_L
		part2	:HealingFlames.part
		POther	Hair
	End

	Event
		EName	9
		Type	Local
		At	Col_R
		part2	:HealingFlames.part
		POther	Hair
	End

	Event
		EName	9
		Type	Local
		At	Hips
		part2	:HealingFlames.part
		POther	Col_R
	End

	Event
		EName	9
		Type	Local
		At	Hips
		part2	:HealingFlames.part
		POther	Col_L
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther	LArmL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
		POther LArmR
	End

	Event
		EName	15
		Type	Local
		At	Chest
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
	End

	Event
		EName	16
		Type	Local
		At	Hair
		part1	:HealingFlames.part
		part1	:HealingFlames3.part
	End

End
