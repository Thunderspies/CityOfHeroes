#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
##Flens Flares
#####################################################

##############################################################################

Condition
	On	Time
	Time	30

	Event
		Ename	Ring
		Type	Local
		At	Hips

		Part	 CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicERing2.part
	End

End

Condition
	On	Time
	Time	31

	Event
		Ename	Ring
		Type	Local
		At	Hips

		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud01.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud02.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicERing.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud03.part

	End

End

###################################################################################

Condition
	On	Time
	Time	43

	Event
		Ename	Ring
		Type	Local
		At	Hips

		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicERing2.part
	End

End

Condition
	On	Time
	Time	44

	Event
		Ename	Ring
		Type	Local
		At	Hips

		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud01.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud02.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicERing.part
		Part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicCloud03.part

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
		Sound mindoverbody 60 60 .8

	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicsLegMotionGlows1.part
		PMagnet	UlegL
		#Sound	glow5_loop 60 30 .12
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicsLegMotionGlows1.part
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	FootR

	End

	Event
		EName	2
		Type	Local
		At	FootR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	LLEGR
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicLegMotionGlows2.part
		PMagnet	LLEGL
	End

####################################################################################
##Original

#####################################################################################

	Event
		EName	3
		Type	Local
		At	ULEGL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part


	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part


	End


#LegR

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows2.part

	End

	Event
		EName	9
		Type	Local
		At	Col_L
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows2.part

	End

	Event
		EName	9
		Type	Local
		At	Col_R
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows2.part

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

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part

		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows.part

	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part

		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows1.part

		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	CustomizablePowers_Pools\Epic_PsionicMastery\Subtractive\PsionicArmMotionGlows.part

	End

End


End


