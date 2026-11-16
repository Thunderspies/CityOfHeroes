#########################################################
##	Fire_Ball
##
FxInfo
LifeSpan	165


#####################################################################################
##Flens Flares
#####################################################

Condition
	On 	cycle
	Time 	64
	Chance	.4

	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		LifeSpan	30
	End

End


Condition
	On 	cycle
	Time 	70
	Chance	.4

	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
		LifeSpan	30
	End


End

Condition
	On 	cycle
	Time 	75
	Chance	.4

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
		LifeSpan	30
	End

End

Condition
	On 	cycle
	Time 	83
	Chance	.4

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
		LifeSpan	30
	End


End

#####################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:ArmMotionGlowsx.part

	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:ArmMotionGlows2x.part
		PMagnet	UlegL
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:ArmMotionGlows2x.part
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:ArmMotionGlowsx.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	:ArmMotionGlowsx.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:ArmMotionGlowsx.part
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:ArmMotionGlowsx.part
		PMagnet	FootR

	End




	Event
		EName	2
		Type	Local
		At	FootR
		part1	:ArmMotionGlows2x.part

	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:ArmMotionGlows2x.part

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
		part1	:ArmMotionGlows2.part

	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:ArmMotionGlows2.part
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:ArmMotionGlows2.part
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:ArmMotionGlows2.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:ArmMotionGlows2.part
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:ArmMotionGlows2.part
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:ArmMotionGlows2.part

	End

End



End


