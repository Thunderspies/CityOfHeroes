#########################################################
##	chill touch hit
FxInfo

Flags    InheritAlpha

lifeSpan	20
##################################
Condition
	On	Time
	Time	0
	
	Event
		EName	1
		Type	Local
		At	chest
		Sound Glow8_loop 70 70 .82
		
	End

#	Event
#		Type	Local
#		At	back
#		BHVR	Behaviors/GhostPivot.bhvr
#		Part3	:Core.part
#		Part2	:CoreFlame2.part
#		Part1	:CoreFlame.part
#		Sound	Terror_loop 70 60 .35
#	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:HealingFlames.part
		
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:HealingFlames.part
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:HealingFlames.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:HealingFlames.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:HealingFlames.part
		PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:HealingFlames.part
		PMagnet FootR
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
	
	End

	Event
		EName	15
		Type	Local
		At	Chest
		part1	:HealingFlames.part
	End

End


End				