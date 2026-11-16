#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Hips
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	Head
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

#####################################################################################
##Flens Flares
#####################################################

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	CustomizeablePowers\Regeneration\FortitudeRaysSmall2.part
		part2	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part3	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part4	CustomizeablePowers\Regeneration\FortitudeDots2.part

		LifeSpan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	CustomizeablePowers\Regeneration\FortitudeRaysSmall2.part
		part2	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part3	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part4	CustomizeablePowers\Regeneration\FortitudeDots2.part

		LifeSpan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	CustomizeablePowers\Regeneration\FortitudeRaysSmall2.part
		part2	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part3	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part4	CustomizeablePowers\Regeneration\FortitudeDots2.part
		
		LifeSpan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	CustomizeablePowers\Regeneration\FortitudeRaysSmall2.part
		part2	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part3	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part4	CustomizeablePowers\Regeneration\FortitudeDots2.part
		
		LifeSpan	25
	End
	

End

Condition
	On 	cycle
	Time 	17
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Regeneration\FortitudeRaysSmall2.part
		part2	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part3	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part4	CustomizeablePowers\Regeneration\FortitudeDots2.part
		
		LifeSpan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Regeneration\FortitudeRaysSmall2.part
		part2	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part3	CustomizeablePowers\Regeneration\FortitudeRing2.part
		part4	CustomizeablePowers\Regeneration\FortitudeDots2.part
		
		LifeSpan	25
	End


End

#####################################################################################


Condition
	On	Time
	Time	58

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part1	CustomizeablePowers\Regeneration\Ring03.part
		
	End

End

Condition
	On	Time
	Time	5
	
	Event
		Ename	BoostMist
		Type	Local
		At	Hips
		
		Part2	CustomizeablePowers\Regeneration\GreenMist.part
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
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	UlegR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows3.part
		PMagnet	LLEGL

	End

	Event
		EName	7
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows3.part
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
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		
		PMagnet UlegL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		
		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		
		#PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		
		#PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		
		#PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		
		#PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
	
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

			