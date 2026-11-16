#############################################################
## Fortitude.fx
#############################################################

FxInfo
LifeSpan	165

#########################################################

Input
	Inpname	Hips
End

#########################################################

#########################################################
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
		part1	CustomizeablePowers\Empathy\FortitudeRaysSmall.part
		part2	CustomizeablePowers\Empathy\FortitudeRing.part
		part3	CustomizeablePowers\Empathy\FortitudeRing.part
		part4	CustomizeablePowers\Empathy\FortitudeDots.part

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
		part1	CustomizeablePowers\Empathy\FortitudeRaysSmall.part
		part2	CustomizeablePowers\Empathy\FortitudeRing.part
		part3	CustomizeablePowers\Empathy\FortitudeRing.part
		part4	CustomizeablePowers\Empathy\FortitudeDots.part
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
		part1	CustomizeablePowers\Empathy\FortitudeRaysSmall.part
		part2	CustomizeablePowers\Empathy\FortitudeRing.part
		part3	CustomizeablePowers\Empathy\FortitudeRing.part
		part4	CustomizeablePowers\Empathy\FortitudeDots.part
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
		part1	CustomizeablePowers\Empathy\FortitudeRaysSmall.part
		part2	CustomizeablePowers\Empathy\FortitudeRing.part
		part3	CustomizeablePowers\Empathy\FortitudeRing.part
		part4	CustomizeablePowers\Empathy\FortitudeDots.part
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
		part1	CustomizeablePowers\Empathy\ArmMotionGlowsx.part

	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2x.part
		PMagnet	UlegL
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2x.part
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Empathy\ArmMotionGlowsx.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Empathy\ArmMotionGlowsx.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Empathy\ArmMotionGlowsx.part
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Empathy\ArmMotionGlowsx.part
		PMagnet	FootR

	End




	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2x.part

	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2x.part

	End

####################################################################################
##Original

#####################################################################################

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

		PMagnet UlegL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part


	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

		#PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

		#PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

		#PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

		#PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Empathy\ArmMotionGlows2.part

	End

End



End


