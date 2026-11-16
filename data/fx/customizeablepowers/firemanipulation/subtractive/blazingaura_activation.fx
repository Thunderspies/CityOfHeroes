#############################################################
## BlazingAura_Activation.fx
#############################################################

FxInfo

#############################################################

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
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Sound fireball1 50 50 .5
	End
End

## FIRST PULSE ###########################################################

Condition
	On	Time
	Time	8

	Event
		EName   eclipse
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\FireAuraOut.part
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	20

	Event
		EName   RadiusFxScalex
		Type	local
		At	root
		Bhvr	CustomizeablePowers\FireManipulation\FireShield.bhvr
		LifeSpan 30
	End

	Event
		Type	Local
		At	RadiusFxScalex
		bhvr	CustomizeablePowers\FireManipulation\Infernobubble4.bhvr
		##Part1	CustomizeablePowers\FireManipulation\FireShieldCloud01B.part
		Part2	CustomizeablePowers\FireManipulation\FireShieldCloud02B.part
		Part4	CustomizeablePowers\FireManipulation\FireShieldSparksB.part
		LifeSpan 30
	End
End

## PULSE ###########################################################

Condition
	On 	cycle
	Time 	40
	Chance	1

	Event
		EName   eclipse
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\FireAuraOut.part
		LifeSpan 30
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\FireManipulation\FireShield.bhvr
		Sound aura6 50 50 .15
		Sound aura5 50 50 .12
		Sound aura7 50 50 .11
		LifeSpan 30
	End

	Event
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\FireManipulation\Infernobubble4.bhvr
		Part1	CustomizeablePowers\FireManipulation\FireShieldCloud01B.part
		Part2	CustomizeablePowers\FireManipulation\FireShieldCloud02B.part
		Part4	CustomizeablePowers\FireManipulation\FireShieldSparksB.part
		LifeSpan 30
	End
End

#############################################################

End