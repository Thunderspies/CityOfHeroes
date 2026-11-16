#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	40

#####################################################################################
##Flens Flares
#####################################################


Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound peaceAOEhit 80.0 80.0 .89
	End
End



Condition
	
	Event
		Type	Local
		At	chest
		
		ChildFx	Powers/ItemsandTempPowers/FlashGrenadeHit.fx
		LifeSpan	60
	End
	
End

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		
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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		
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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		
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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		
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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		
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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		
		LifeSpan	25
	End


End

#####################################################################################

Condition
	On	Time
	Time	38

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part1 POWERS/Regeneration/Ring03.part
		
	End

End

Condition
	On	Time
	Time	0
	
	Event
		EName	3
		Type	Local
		At	Hips
		part1	Powers/Regeneration/MomentOfGlorySparkliesTrail.part
		part2	:MotionGlows.part
		Part3	:InvincibilityLightRays.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UlegL
		
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	FootR

	End




	Event
		EName	2
		Type	Local
		At	FootR
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

####################################################################################
##Original

#####################################################################################
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet UlegL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

End



End	


			