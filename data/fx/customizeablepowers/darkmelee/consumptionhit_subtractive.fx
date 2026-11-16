#############################################################
## ConsumptionHit_Subtractive.fx
#############################################################

FxInfo
lifespan	70

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

		Part	CustomizeablePowers\DarkMelee\SubtractiveFlash1.part
		Part	CustomizeablePowers\DarkMelee\CreateDarknessPuddle_Subtractive_Local.part
		Part1	CustomizeablePowers\DarkMelee\CreateDarknessPuddle_Subtractive.part
		Part2	CustomizeablePowers\DarkMelee\CreateDarknessPuddle2_Subtractive.part

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
		part1	CustomizeablePowers\DarkMelee\endurance01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part1	CustomizeablePowers\DarkMelee\enduranceBodyGlows.part
		part2	CustomizeablePowers\DarkMelee\enduranceBodyGlowsFlat.part
	End

	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part1	CustomizeablePowers\DarkMelee\enduranceMotionGlows.part
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part1	CustomizeablePowers\DarkMelee\enduranceMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	T_FootL
		part1	CustomizeablePowers\DarkMelee\enduranceBodyGlows.part
		part2	CustomizeablePowers\DarkMelee\enduranceBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part1	CustomizeablePowers\DarkMelee\enduranceMotionGlows.part
		PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part1	CustomizeablePowers\DarkMelee\enduranceMotionGlows.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	T_Hair
		part2	CustomizeablePowers\DarkMelee\enduranceHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part1	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows2.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part1	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows2.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part1	CustomizeablePowers\DarkMelee\enduranceArmGlows.part

	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part1	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows2.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part1	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\DarkMelee\enduranceArmMotionGlows2.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part1	CustomizeablePowers\DarkMelee\enduranceArmGlows.part

	End



End


Condition
	On	Time
	Time	10


	Event

		ENAME	Base
		Type	start
		At	root

		Part3	CustomizeablePowers\DarkMelee\ConsumptionRadiusRing.Part
		Part5	CustomizeablePowers\DarkMelee\ConsumptionRadiusRingThin.Part
		#Sound 	Blackhole4 80 30 .75
		LifeSpan	10
	End

End


End