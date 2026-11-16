#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
	End
End


Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Sound firesword_loop 50 50 .14
	End

	Event
		Type	Local
		At	Origin
		Sound fireball1 50 50 .5
	End
End




Condition
	On	Time
	Time	20

	Event
		EName   RadiusFxScalex
		Type	local
		At	root
		Bhvr	CustomizeablePowers\FieryAura\FireShield.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScalex
		bhvr	CustomizeablePowers\FieryAura\Infernobubble4.bhvr
		Part1	CustomizeablePowers\FieryAura\FireShieldCloud01B.part
		Part2	CustomizeablePowers\FieryAura\FireShieldCloud02B.part
		Part4	CustomizeablePowers\FieryAura\FireShieldSparksB.part

		LifeSpan	30

	End
End

Condition
	On	Time
	Time	8

	Event
		EName   eclipse
		Type	local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FieryAura\FireAuraOut.part
		LifeSpan	30
	End

End


################################################################################################

Condition
	On	Time
	Time	15

	Event
		EName	delay
		Type	Local
		At	root

	End

End

Condition

	On 	cycle
	Time 	40
	Chance	1

	Event
		EName   eclipse
		Type	local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FieryAura\FireAuraOut.part
		LifeSpan	30
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	delay
		Bhvr	CustomizeablePowers\FieryAura\FireShield.bhvr
		Sound aura6 50 50 .15
		Sound aura5 50 50 .12
		Sound aura7 50 50 .11
		LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\FieryAura\Infernobubble4.bhvr
		Part1	CustomizeablePowers\FieryAura\FireShieldCloud01B.part
		Part2	CustomizeablePowers\FieryAura\FireShieldCloud02B.part
		Part4	CustomizeablePowers\FieryAura\FireShieldSparksB.part

		LifeSpan	30

	End

End

End