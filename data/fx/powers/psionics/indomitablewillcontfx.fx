#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
##Flens Flares
#####################################################

Condition
	On 	cycle
	Time 	3
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:FortitudeRaysSmall2a.part
		part2	:FortitudeRing2a.part
		part3	:FortitudeRing2a.part
		part4	:FortitudeDots2a.part

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
		part1	:FortitudeRaysSmall2a.part
		part2	:FortitudeRing2a.part
		part3	:FortitudeRing2a.part
		part4	:FortitudeDots2a.part

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
		part1	:FortitudeRaysSmall2a.part
		part2	:FortitudeRing2a.part
		part3	:FortitudeRing2a.part
		part4	:FortitudeDots2a.part

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
		part1	:FortitudeRaysSmall2a.part
		part2	:FortitudeRing2a.part
		part3	:FortitudeRing2a.part
		part4	:FortitudeDots2a.part

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
		part1	:FortitudeRaysSmall2a.part
		part2	:FortitudeRing2a.part
		part3	:FortitudeRing2a.part
		part4	:FortitudeDots2a.part

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
		part1	:FortitudeRaysSmall2a.part
		part2	:FortitudeRing2a.part
		part3	:FortitudeRing2a.part
		part4	:FortitudeDots2a.part

		lifespan	25
	End


End
##############################################################################

Condition
	On	Time
	Time	31

	Event
		Ename	Ring
		Type	Local
		At	Hips
		
		Part	:PsionicCloud01a.part
		Part	:PsionicCloud02a.part
		Part	:PsionicCloud03a.part
	
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
		
		Part	:PsionicCloud01a.part
		Part	:PsionicCloud02a.part
		Part	:PsionicCloud03a.part
	
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
		Sound aura_activate 60 60 .5
			
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	:PsionicsLegMotionGlows1a.part
		PMagnet	UlegL
		Sound auraglow6_loop 60 60 .25
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	:PsionicsLegMotionGlows1a.part
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part	:PsionicLegMotionGlows2a.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part	:PsionicLegMotionGlows2a.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	:PsionicLegMotionGlows2a.part
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:PsionicLegMotionGlows2a.part
		PMagnet	FootR

	End

	Event
		EName	2
		Type	Local
		At	FootR
		part	:PsionicLegMotionGlows2a.part
		PMagnet	LLEGR
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part	:PsionicLegMotionGlows2a.part
		PMagnet	LLEGL
	End

####################################################################################
##Original

#####################################################################################
	
	Event
		EName	3
		Type	Local
		At	ULEGL
		part	:PsionicArmMotionGlows1a.part
		

	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part	:PsionicArmMotionGlows1a.part
		

	End


#LegR

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part	:PsionicArmMotionGlows2a.part
		
	End

	Event
		EName	9
		Type	Local
		At	Col_L
		part	:PsionicArmMotionGlows2a.part
		
	End
	
	Event
		EName	9
		Type	Local
		At	Col_R
		part	:PsionicArmMotionGlows2a.part
		
	End
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part	:PsionicArmMotionGlows1a.part
		
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	:PsionicArmMotionGlows1a.part
		
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	:PsionicArmMotionGlowsa.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	:PsionicArmMotionGlows1a.part
		
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	:PsionicArmMotionGlows1a.part
		
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	:PsionicArmMotionGlowsa.part
	
	End

End


End	


			