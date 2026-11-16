#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
LifeSpan	60
#####################################################################################


Condition
	On	Time
	Time	58

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part1 POWERS/Regeneration/Ring03.part
		
	End

End

Condition
	On	Time
	Time	5
	
	Event
		Ename	BoostMist
		Type	Local
		At	Hips
		
		Part2 :GreenMist.part
		Sound regen8c 60 60 .76
		LifeSpan	60
	End

End

Condition
	On	Time
	Time	0
	
	Event
		EName	3
		Type	Local
		At	UlegL
		part1	:RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	UlegR
		part1	:RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:RegenBodyGlows2.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	:RegenBodyGlows2.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	FootL
		part1	:RegenBodyGlows3.part
		PMagnet	LLEGL

	End

	Event
		EName	7
		Type	Local
		At	FootR
		part1	:RegenBodyGlows3.part
		PMagnet	LLEGR

	End

####################################################################################
##Original
##RegenBodyGlows2.part
#####################################################################################
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:RegenBodyGlows.part
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:RegenBodyGlows.part
		

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:RegenBodyGlows.part
		
		PMagnet UlegL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:RegenBodyGlows.part
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:RegenBodyGlows.part
		

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:RegenBodyGlows.part
		
		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:RegenBodyGlows.part
		
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:RegenBodyGlows.part
		
		#PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:RegenBodyGlows.part
		
		#PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:RegenBodyGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:RegenBodyGlows.part
		
		#PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:RegenBodyGlows.part
		
		#PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:RegenBodyGlows.part
	
	End

End
###################################################################################
##HealingPart
#############################################################################

#Condition
#	Time 	20
#			
#	Event
#		Ename	Mushroom
#		Type	Local
#		At	Hips
#		Part1 POWERS/Regeneration/Glow01.part
#		Lifespan	30
#		bhvr	Behaviors/GenericParticleFade.bhvr
#		Sound	heal1_loop 60 30 .45
#	End
#
#
#End
#
#Condition
#	On 	cycle
#	Time 	90
#	Chance	1
#	
#	Event
#		Ename	Mushroom
#		Type	Local
#		At	Hips
#		Part1 :Glow01.part
#		Lifespan	30
#		bhvr	Behaviors/GenericParticleFade.bhvr
#	End
#
#
#End	

End

			