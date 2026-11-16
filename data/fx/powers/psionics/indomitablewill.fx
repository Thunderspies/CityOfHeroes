#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
##Flens Flares
#####################################################
lifeSpan	30
##############################################################################

Condition
	On 	cycle
	Time 	3
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	3
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	3
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		lifespan	25
	End
	
End


Condition
	On 	cycle
	Time 	3
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		lifespan	25
	End
	

End

Condition
	On 	cycle
	Time 	3
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		lifespan	25
	End


End
######################################################################################

Condition
	On	Time
	Time	31

	Event
		Ename	Ring
		Type	Local
		At	Hips
		
		Part	:PsionicCloud01.part
		Part	:PsionicCloud02.part
		#Part	:PsionicERing.part
		Part	:PsionicCloud03.part
		LifeSpan	20
	End

End

###################################################################################

Condition
	On	Time
	Time	44

	Event
		Ename	Ring
		Type	Local
		At	Hips
		
		Part	:PsionicCloud01.part
		Part	:PsionicCloud02.part
		#Part	:PsionicERing.part
		Part	:PsionicCloud03.part
		LifeSpan	20
	
	End

End

#################################################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	3
		Type	Local
		At	Hips
		Sound regen5b 60 60 .5
		LifeSpan	20
			
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	:PsionicsLegMotionGlows1.part
		PMagnet	UlegL
		Sound glow5_loop 60 60 .12
		LifeSpan	20
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	:PsionicsLegMotionGlows1.part
		PMagnet	UlegR
		LifeSpan	20
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part	:PsionicLegMotionGlows2.part
		PMagnet	ULEGL
		LifeSpan	20

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part	:PsionicLegMotionGlows2.part
		PMagnet	ULEGR
		LifeSpan	20


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	:PsionicLegMotionGlows2.part
		PMagnet	FootL
		LifeSpan	20

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:PsionicLegMotionGlows2.part
		PMagnet	FootR
		LifeSpan	20

	End

	Event
		EName	2
		Type	Local
		At	FootR
		part	:PsionicLegMotionGlows2.part
		PMagnet	LLEGR
		LifeSpan	20
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part	:PsionicLegMotionGlows2.part
		PMagnet	LLEGL
		LifeSpan	20
	End

####################################################################################
##Original

#####################################################################################
	
	Event
		EName	3
		Type	Local
		At	ULEGL
		part	:PsionicArmMotionGlows1.part
		LifeSpan	20		

	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part	:PsionicArmMotionGlows1.part
		LifeSpan	20		

	End

###############################################################################

	Event
		Type	Local
		At	Head
		part	:PsionicArmMotionGlows2.part
		LifeSpan	20
		
	End
	
	Event
		Type	Local
		At	Col_L
		part	:PsionicArmMotionGlows2.part
		LifeSpan	20
		
	End


	Event
		Type	Local
		At	Col_R
		part	:PsionicArmMotionGlows2.part
		LifeSpan	20
		
	End

	Event
		Type	Local
		At	Head
		part	:HeadHalo.part
		LifeSpan	40
		
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part	:PsionicArmMotionGlows1.part
		
		PMagnet LArmL
		LifeSpan	20

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	:PsionicArmMotionGlows1.part
		
		PMagnet WepL
		LifeSpan	20
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	:PsionicArmMotionGlows.part
		LifeSpan	20
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	:PsionicArmMotionGlows1.part
		
		PMagnet LArmR
		LifeSpan	20

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	:PsionicArmMotionGlows1.part
		
		PMagnet WepR
		LifeSpan	20
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	:PsionicArmMotionGlows.part
		LifeSpan	20
	
	End

End


End	


			