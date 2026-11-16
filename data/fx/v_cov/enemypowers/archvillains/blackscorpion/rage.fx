#########################################################
##	chill touch hit
FxInfo

#LifeSpan	100

##################################


Condition
	On	Time
	Time	18
	
	Event
		EName	1
		Type	Local
		At	Hips
#		part1	:endurance01.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		
		BhvrOverride
			PositionOffset	.25 0 0
		End

		part1	:enduranceMotionGlowsFeet.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:enduranceMotionGlows.part

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:enduranceMotionGlowsLegs.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		
		BhvrOverride
			PositionOffset	-.25 0 0
		End

		part1	:enduranceMotionGlowsFeet.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:enduranceMotionGlows.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:enduranceMotionGlowsLegs.part
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part2	:enduranceHeadGlows.part
	End

	Event
		EName	LeftShoulder
		Type	Local
		At	Col_L
		
		BhvrOverride
			PositionOffset	.5 .5 .8
		End

		part2	:enduranceHeadGlows.part
	End
	
	Event
		EName	RightShoulder
		Type	Local
		At	Col_R
		
		BhvrOverride
			PositionOffset	-.5 .3 .8
		End

		part2	:enduranceHeadGlows.part
	End
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
#		part1	:enduranceArmMotionGlows.part
		part2	:enduranceArmMotionGlows3.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:enduranceArmMotionGlows.part
#		part2	:enduranceArmMotionGlows2.part
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:enduranceArmGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		BhvrOverride
			PositionOffset	1 .03 0
		End

		part1	:enduranceArmMotionGlows4.part

	End

	Event
		EName	13
		Type	Local
		At	UArmL
		BhvrOverride
			PositionOffset	-1 -.03 0
		End

		part1	:enduranceArmMotionGlows5.part

	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part2	:enduranceArmMotionGlows2.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:enduranceArmMotionGlows.part
#		part2	:enduranceArmMotionGlows2.part
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:enduranceArmGlows.part
	
	End



End

#Condition
#	On	Time
#	Time	60
#	
#	Event
#		EName	all
#		Type	Destroy
#				
#	End
#
#End


End				