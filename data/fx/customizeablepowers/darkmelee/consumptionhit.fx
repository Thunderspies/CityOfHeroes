#############################################################
## ConsumptionHit.fx
#############################################################

FxInfo
Lifespan	70

#############################################################

Input
	Inpname	Hips
End

##########################################################

Condition

	Event
		EName	darkskirt
		Type	Local #Start
		At	ROOT
		Part	CustomizeablePowers\DarkMelee\CreateDarknessPuddle.part
		Part	CustomizeablePowers\DarkMelee\CreateDarknessPuddle2.part
		LifeSpan	13
	End

End

Condition
	On 	Time
	Time 	4

	Event
		EName	darkskirt
		Type	Local #Start
		At	ROOT
		Part	CustomizeablePowers\DarkMelee\RedRaysUP.part
		Sound ConsumptionHit2 70 70 .88
		LifeSpan	27
	End

	Event

		ENAME	hookupW/Spin
		Type	posit
		At	root

		BHVR	CustomizeablePowers\DarkMelee\SplatRotate.bhvr
		Sound Tar_loop 80 80 .53
	End

	Event

		ENAME	BaseTrail
		Type	local
		At	root

		Part	CustomizeablePowers\DarkMelee\CreateDarknessTrail.part
		Part	CustomizeablePowers\DarkMelee\CreateDarknessTrail2.part
		lifespan	20

	End

	Event
		EName	darkskirt
		Type	Local #Start
		At	ROOT

		Part	CustomizeablePowers\DarkMelee\CreateDarknessPuddleFlat.part
		Part	CustomizeablePowers\DarkMelee\CreateDarknessPuddle2Flat.part
		LifeSpan	17
	End

End


#############################################################################
##EndurenceGlow
############################################################3

Condition
	On	Time
	Time	0

	Event
		EName	1
		Type	Local
		At	T_Hips
		part	CustomizeablePowers\Empathy\endurance01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part	CustomizeablePowers\Empathy\enduranceBodyGlows.part
		part	CustomizeablePowers\Empathy\enduranceBodyGlowsFlat.part
	End

	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	T_FootL
		part	CustomizeablePowers\Empathy\enduranceBodyGlows.part
		part	CustomizeablePowers\Empathy\enduranceBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	T_Hair
		part	CustomizeablePowers\Empathy\enduranceHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part	CustomizeablePowers\Empathy\enduranceArmGlows.part

	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part	CustomizeablePowers\Empathy\enduranceArmGlows.part

	End



End

##################################
##BlackHole
######################################################################3

Condition
	On	Time
	Time	10


	Event

		ENAME	Base
		Type	start
		At	root
		Part	CustomizeablePowers\DarkMelee\ConsumptionRadiusRing.Part
		Part	CustomizeablePowers\DarkMelee\ConsumptionRadiusRingThin.Part
		LifeSpan	10
	End

End


End