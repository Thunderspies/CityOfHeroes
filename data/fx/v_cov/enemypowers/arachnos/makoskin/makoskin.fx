#########################################################
##	Fire_Ball
##
FxInfo


#####################################################################################
## lens Flares
#####################################################

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

		lifespan	25
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

		lifespan	25
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

		lifespan	25
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

		lifespan	25
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

		lifespan	25
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

		lifespan	25
	End


End

#####################################################################################

Condition
	On	Time
	Time	15

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part	:Glow01.part
	End

End

Condition
	On	Time
	Time	28

	Event
		Ename	Ring
		Type	Local
		At	Hips
		
		Part	:ERing02.part
		Part	:Cloud02.part
		Part	:Shield01.part
	
	End

	Event
		Ename	Logo
		Type	Local
		At	Chest
		Part	:Logo01.part
	End

	Event
		Ename	Logo
		Type	Local
		At	Waist
		BhvrOverride
			PositionOffset	0 .5 0
		End
		Part	:Logo02.part
	End

End

Condition
	On	Death

	Event
		Ename	Ring
		Type	Local
		At	Hips
		
		Part	:ERing01.part
		Part	:Cloud01.part

		LifeSpan	1
	
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
		part1	:LegMotionGlows2.part
		Sound regen5b 60 60 .5
			
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:LegMotionGlows2.part
		PMagnet	UlegL
		Sound glow5_loop 60 60 .12
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:LegMotionGlows2.part
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:LegMotionGlows2.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	:LegMotionGlows2.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:LegMotionGlows2.part
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:LegMotionGlows2.part
		PMagnet	FootR

	End




	Event
		EName	2
		Type	Local
		At	FootR
		part1	:LegMotionGlows2.part
		
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:LegMotionGlows2.part
		
	End

####################################################################################
##Original

#####################################################################################
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:ArmMotionGlows2.part
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:ArmMotionGlows2.part
		

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:ArmMotionGlows2.part
		
		PMagnet UlegL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:ArmMotionGlows2.part
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:ArmMotionGlows2.part
		

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:ArmMotionGlows2.part
		
		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:MainMotionGlows2.part
		
	End

	Event
		EName	16
		Type	Local
		At	Chest
		part1	:MainMotionGlows2.part
		
	End

	Event
		EName	17
		Type	Local
		At	Neck
		part1	:MainMotionGlows2.part
		PMagnet UarmL
		
	End

	Event
		EName	18
		Type	Local
		At	Neck
		part1	:MainMotionGlows2.part
		PMagnet UarmR
		
	End

	Event
		EName	19
		Type	Local
		At	Neck
		part1	:LegMotionGlows2.part
		
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:ArmMotionGlows2.part
		
		#PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:ArmMotionGlows2.part
		
		#PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	HandL
		part1	:ArmMotionGlows2.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:ArmMotionGlows2.part
		
		#PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:ArmMotionGlows2.part
		
		#PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	HandR
		part1	:ArmMotionGlows2.part
	
	End

End


End	


			