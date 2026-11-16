#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
##Flens Flares
#####################################################
#lifeSpan	30
##############################################################################

Condition
	On 	cycle
	Time 	3
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LarmL
		part1	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRaysSmall2.part
		part2	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part3	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part4	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeDots2.part

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
		part1	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRaysSmall2.part
		part2	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part3	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part4	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeDots2.part

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
		part1	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRaysSmall2.part
		part2	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part3	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part4	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeDots2.part

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
		part1	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRaysSmall2.part
		part2	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part3	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part4	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeDots2.part

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
		part1	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRaysSmall2.part
		part2	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part3	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part4	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeDots2.part

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
		part1	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRaysSmall2.part
		part2	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part3	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeRing2.part
		part4	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\FortitudeDots2.part

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

		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud01.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud02.part
		#Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicERing.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud03.part
		#LifeSpan	20
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

		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud01.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud02.part
		#Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicERing.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud03.part
		#LifeSpan	20

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
		#LifeSpan	20

	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicsLegMotionGlows1.part
		PMagnet	UlegL
		Sound glow5_loop 60 60 .12
		LifeSpan	420
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicsLegMotionGlows1.part
		PMagnet	UlegR
		#LifeSpan	20
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	ULEGL
		#LifeSpan	20

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	ULEGR
		#LifeSpan	20


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	FootL
		#LifeSpan	20

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	FootR
		#LifeSpan	20

	End

	Event
		EName	2
		Type	Local
		At	FootR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	LLEGR
		#LifeSpan	20
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	LLEGL
		#LifeSpan	20
	End

####################################################################################
##Original

#####################################################################################

	Event
		EName	3
		Type	Local
		At	ULEGL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part
		#LifeSpan	20

	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part
		#LifeSpan	20

	End

###############################################################################

	Event
		Type	Local
		At	Head
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows2.part
		#LifeSpan	20

	End

	Event
		Type	Local
		At	Col_L
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows2.part
		#LifeSpan	20

	End


	Event
		Type	Local
		At	Col_R
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows2.part
		#LifeSpan	20

	End

	Event
		Type	Local
		At	Head
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\HeadHalo.part
		LifeSpan	40

	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part

		PMagnet LArmL
		#LifeSpan	20

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part

		PMagnet WepL
		#LifeSpan	20
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows.part
		#LifeSpan	20

	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part

		PMagnet LArmR
		#LifeSpan	20

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part

		PMagnet WepR
		#LifeSpan	20
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows.part
		#LifeSpan	20

	End

End


End


