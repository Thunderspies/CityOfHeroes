#############################################################
## HealingFlames.fx
#############################################################

FxInfo
LifeSpan	105

#######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	RingdownHookUp
		Type	local
		At	root
		Geom	RootToHeadAdjustment

	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	ring
		Type	local
		At	root
		bhvr	CustomizeablePowers\FieryAura\lifespan2.bhvr
		Part1	:HealingFlame01.part
		Part2	:WilloWispSpark.part
		Part3	:HealingFlame02.part
		Part3	:HealingFlame02_Alpha.part

		LifeSpan	70
	End

End

#########################################################################3
##RingsUP
##########################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\Subtractive\StaminaRingUp.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		Sound heal1 70 70 .88

	End

End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\Subtractive\StaminaRingUp.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End

Condition
	On	Time
	Time	25

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\Subtractive\StaminaRingUp.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End

Condition
	On	Time
	Time	30

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\Subtractive\StaminaRingUp.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End



#####################################################################
#StaminaRingDown.part
####################################################

Condition
	On	Time
	Time	55

	Event
		EName	Ring
		Type	Local
		At	RingdownHookUp
		altpiv	1
		part1	:StaminaRingDown.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End

Condition
	On	Time
	Time	65

	Event
		EName	Ring
		Type	Local
		At	RingdownHookUp
		altpiv	1
		part1	:StaminaRingDown.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End

Condition
	On	Time
	Time	70

	Event
		EName	Ring
		Type	Local
		At	RingdownHookUp
		altpiv	1
		part1	:StaminaRingDown.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End

Condition
	On	Time
	Time	80

	Event
		EName	Ring
		Type	Local
		At	RingdownHookUp
		altpiv	1
		part1	:StaminaRingDown.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End

Condition
	On	Time
	Time	85

	Event
		EName	Ring
		Type	Local
		At	RingdownHookUp
		altpiv	1
		part1	:StaminaRingDown.part
		BHVR	Behaviors\GenericParticleFade.bhvr

	End

End

###################################################################################

Condition
	On	Time
	Time	18


	Event
		EName	2
		Type	Local
		At	FootR
		part	:HealingBodyGlows.part
		part	:HealingBodyGlowsFlat.part
		BHVR	Behaviors\GenericParticleFade.bhvr
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part	:HealingMotionGlows.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	:HealingMotionGlows.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part	:HealingBodyGlows.part
		part	:HealingBodyGlowsFlat.part
		BHVR	Behaviors\GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part	:HealingMotionGlows.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:HealingMotionGlows.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet FootR
	End



	Event
		EName	9
		Type	Local
		At	Hair
		part2	:HealingHeadGlows.part
		BHVR	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part	:HealingArmMotionGlows.part
		part	:HealingArmMotionGlows2.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	:HealingArmMotionGlows.part
		part	:HealingArmMotionGlows2.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part	:HealingArmGlows.part
		BHVR	Behaviors\GenericParticleFade.bhvr
	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part	:HealingArmMotionGlows.part
		part	:HealingArmMotionGlows2.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	:HealingArmMotionGlows.part
		part	:HealingArmMotionGlows2.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part	:HealingArmGlows.part
		BHVR	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On	Time
	Time	45

	Event
		EName	15
		Type	Local
		At	Hips
		Part	:FireCoreSparks2.part
		part	:HealingFlameGlow.part
		BHVR	Behaviors\GenericParticleFade.bhvr
	End

End


##########################################################################################
##Healing Flames Ring
#######################################################

Condition
	On 	time
	Time 	83

	Event
		EName 	ring
		Type	local
		At	root
		bhvr	CustomizeablePowers\FieryAura\lifespan2.bhvr
		Part1	:HealingFlame01.part
		Part2	:WilloWispSpark.part
		Part3	:HealingFlame02.part
		LifeSpan	40
	End

End


################################################################
##Round1
##############################################


Condition
	On 	Time
	Time 	20

	Event
		EName 	Fire1
		Type	local
		At	root
		Anim	FX_WilloWisps
		Sound healingflame 100 100 .88
		LifeSpan	70
	End

	Event
		EName 	FireChest
		Type	local
		At	Fire1
		Animpiv	chest
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireFace
		Type	local
		At	Fire1
		Animpiv	Face
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireNeck
		Type	local
		At	Fire1
		Animpiv	Neck
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireWaist
		Type	local
		At	Fire1
		Animpiv	Waist
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

End

################################################################
##Round2
##############################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Fire2
		Type	local
		At	root
		Anim	FX_WilloWisps
		Sound healingflame 100 100 .75
		LifeSpan	65
	End

	Event
		EName 	FireChest2
		Type	local
		At	Fire2
		Animpiv	chest
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireFace2
		Type	local
		At	Fire2
		Animpiv	Face
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireNeck2
		Type	local
		At	Fire2
		Animpiv	Neck
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireWaist2
		Type	local
		At	Fire2
		Animpiv	Waist
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

End

################################################################
##Round3
##############################################

Condition
	On 	Time
	Time 	37

	Event
		EName 	Fire3
		Type	local
		At	root
		LifeSpan	65
	End

	Event
		EName 	FireChest3
		Type	local
		At	Fire3
		Animpiv	chest
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part

		LifeSpan	65
	End

	Event
		EName 	FireFace3
		Type	local
		At	Fire3
		Animpiv	Face
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireNeck3
		Type	local
		At	Fire3
		Animpiv	Neck
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

	Event
		EName 	FireWaist3
		Type	local
		At	Fire3
		Animpiv	Waist
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part
		LifeSpan	65
	End

End

################################################################
##Round4
##############################################

Condition
	On 	Time
	Time 	40

	Event
		EName 	Fire4
		Type	local
		At	root
		Anim	FX_WilloWisps
		LifeSpan	65
	End

	Event
		EName 	FireChest4
		Type	local
		At	Fire4
		Animpiv	chest
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part

		LifeSpan	65
	End

	Event
		EName 	FireFace4
		Type	local
		At	Fire4
		Animpiv	Face
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part

		LifeSpan	65
	End

	Event
		EName 	FireNeck4
		Type	local
		At	Fire4
		Animpiv	Neck
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part

		LifeSpan	65
	End

	Event
		EName 	FireWaist4
		Type	local
		At	Fire4
		Animpiv	Waist
		Part1	:HealingFlame.part
		Part1	:HealingFlame_Alpha.part

		LifeSpan	65
	End

End




Condition
	On 	Time
	Time 	90

	Event
		EName 	Ring
		Type	Destroy

	End

End

End
