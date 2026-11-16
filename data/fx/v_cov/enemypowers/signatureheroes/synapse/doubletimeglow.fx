#########################################################
##	Fire_Ball
##
FxInfo


#####################################################################################

Condition
	On	Time
	Time	45
		
	Event
		EName	Powerup
		Type	Destroy
	End
End

Condition
	On	Time
	Time	0
		
	Event
		EName	Powerup
		Type	Local
		At	Hips
		ChildFX :DoubleTimeActivate.fx
	End

End

Condition
	On	Time
	Time	20
				
	Event
		EName	A
		Type	Local
		At	Chest
		part1	:MotionGlowsWHITE2.part
		part2	:MotionGlowsWHITE.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 .3 0
		End
		#PMagnet	Neck
	End
			
	Event
		EName	B
		Type	Local
		At	Chest
		part2	:MotionGlowsRED.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet	Hips
	End
				
	Event
		EName	B
		Type	Local
		At	Chest
		part2	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UarmR
	End
				
	Event
		EName	B
		Type	Local
		At	Chest
		part2	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UarmL
	End
	
	Event
		EName	C
		Type	Local
		At	Waist
		part2	:MotionGlowsRED.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	C
		Type	Local
		At	Chest
		part2	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	Waist
	End

	Event
		EName	D
		Type	Local
		At	Hips
		part2	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	Chest
	End

	Event
		EName	E
		Type	Local
		At	Hips
		part1	:MotionGlowsRED.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UlegL
	End

	Event
		EName	F
		Type	Local
		At	Hips
		part1	:MotionGlowsRED.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	:MotionGlows2.part
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
		EName	4
		Type	Local
		At	LLEGL
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -.5 -.1
		End
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -.5 -.1
		End
		PMagnet	FootR


	End



	Event
		EName	2
		Type	Local
		At	FootR
		part1	:MotionGlowsWHITE2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	ULEGR
		
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:MotionGlowsWHITE2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	ULEGL
	End


	Event
		EName	2
		Type	Local
		At	FootR
		part1	:MotionGlowsWHITE.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:MotionGlowsWHITE.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

####################################################################################
##Original

#####################################################################################
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:MotionGlowsWHITE2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End

	Event
		EName	4
		Type	Local
		At	ULEGL
		part1	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegL
	End

	Event
		EName	4
		Type	Local
		At	ULEGL
		part1	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -.6 -.1
		End
		PMagnet LlegL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:MotionGlowsWHITE2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End


#LegR


	Event
		EName	7
		Type	Local
		At	ULEGR
		part1	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR
	End

	Event
		EName	7
		Type	Local
		At	ULEGR
		part1	:MotionGlowsRED2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -.6 -.1
		End
		PMagnet LlegR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 .2 0
		End
	End

	Event
		EName	9
		Type	Local
		At	Head
		part1	:MotionGlowsWHITE2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 .2
		End
	End

	Event
		EName	9
		Type	Local
		At	Neck
		part1	:MotionGlows.part
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
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	HandL
		part1	:MotionGlowsWHITE2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	HandR
		part1	:MotionGlowsWHITE2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

End



End	


			