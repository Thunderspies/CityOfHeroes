#############################################################
## ConsumptionHit.fx
#############################################################

FxInfo
lifespan	47

#########################################################

Input
	InpName	HIPS
End

##########################################################
Condition

	Event
		EName	darkskirt
		Type	Local #Start
		At	ROOT

		#Part	:SubtractiveFlash1.part
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part

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
		Part	:RedRaysUP.part
		Sound ConsumptionHit2 70 70 .88
		LifeSpan	27
	End

	Event

		ENAME	hookupW/Spin
		Type	posit
		At	root

		BHVR	:SplatRotate.bhvr
		Sound Tar_loop 80 80 .53
	End

	Event

		ENAME	BaseTrail
		Type	local
		At	root

		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		lifespan	20

	End

	Event
		EName	darkskirt
		Type	Local #Start
		At	ROOT

		Part1	:CreateDarknessPuddleFlat.part
		Part2	:CreateDarknessPuddle2Flat.part
		LifeSpan	17
	End

End

#Condition
#	On	cycle
#	Time	5
#
#	Event
#
#		ENAME	BaseTrail
#		Type	start
#		At	hookupW/Spin
#
#		BHVR	:Darktendril.bhvr
#
#		Splat	DarknessSplat.tga
#
#		LifeSpan	30
#
#	End
#
#	Event
#
#		ENAME	BaseTrail
#		Type	start
#		At	hookupW/Spin
#
#		BHVR	:DarktendrilIn.bhvr
#
#		Splat	DarknessSplat2.tga
#
#		LifeSpan	30
#	End
#
#	Event
#
#		ENAME	BaseTrail
#		Type	start
#		At	hookupW/Spin
#
#		BHVR	:DarkSplat2.bhvr
#
#		Splat	Ember02.tga
#		LifeSpan	10
#	End
#
#End


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
		part1	Powers/Healing/endurance01.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part1	Powers/Healing/enduranceBodyGlows.part
		part2	Powers/Healing/enduranceBodyGlowsFlat.part
	End

	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	T_FootL
		part1	Powers/Healing/enduranceBodyGlows.part
		part2	Powers/Healing/enduranceBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	T_Hair
		part2	Powers/Healing/enduranceHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part1	Powers/Healing/enduranceArmGlows.part

	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part1	Powers/Healing/enduranceArmGlows.part

	End



End


Condition
	On	Time
	Time	70

	Event
		EName	1
		Type	Destroy

	End

	Event
		EName	2
		Type	Destroy

	End

	Event
		EName	3
		Type	Destroy

	End

	Event
		EName	4
		Type	Destroy

	End

	Event
		EName	5
		Type	Destroy

	End

	Event
		EName	6
		Type	Destroy

	End

	Event
		EName	7
		Type	Destroy

	End

	Event
		EName	9
		Type	Destroy

	End

	Event
		EName	10
		Type	Destroy

	End

	Event
		EName	11
		Type	Destroy

	End

	Event
		EName	12
		Type	Destroy

	End

	Event
		EName	13
		Type	Destroy

	End

	Event
		EName	14
		Type	Destroy

	End

	Event
		EName	15
		Type	Destroy

	End

End

##################################
##BlackHole
######################################################################3


#Condition
#	On	cycle
#	Time	10
#
#
#	Event
#
#		ENAME	BaseTrail
#		Type	start
#		At	T_Root
#
#		BHVR	:DarkSplatShrink.bhvr
#
#		Splat	generic_Ring.tga
#		LifeSpan	10
#	End
#
#End


Condition
	On	Time
	Time	10


	Event

		ENAME	Base
		Type	start
		At	root

		Part3	:ConsumptionRadiusRing.Part
		Part5	:ConsumptionRadiusRingThin.Part
		#Sound 	Blackhole4 80 30 .75
		LifeSpan	10
	End

End


End