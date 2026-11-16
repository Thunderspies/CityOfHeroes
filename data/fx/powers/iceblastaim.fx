#############################################################
## IceBlastAim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	CHEST
		Bhvr 	Powers\icearmor.bhvr
		Part	Powers\Frost2.part
		part1   Powers\bodychillbig.part
		part1   Powers\SnowBurst.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	Hips
		Bhvr 	Powers\icearmor.bhvr
		part1   Powers\bodychillbig.part
		part1   Powers\SnowBurst.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	Powers\icearmor.bhvr
		part1   Powers\bodychillbig.part
		part1   Powers\SnowBurst.part
		Part	Powers\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	Powers\icearmor.bhvr
		part1   Powers\bodychillbig.part
		part1   Powers\SnowBurst.part
		Part	Powers\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	Powers\icearmor.bhvr
		part1   Powers\bodychillbig.part
		part1   Powers\SnowBurst.part
		Part	Powers\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	Powers\icearmor.bhvr
		part1   Powers\bodychillbig.part
		part1   Powers\SnowBurst.part
		Part	Powers\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   Powers\Snowfall.part
		Part	Powers\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   Powers\Snowfall.part
		Part	Powers\FrostSmall.part
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	UlegL
		Bhvr 	Powers\icearmor.bhvr
		Part	Powers\FrostSmall.part
		LifeSpan 17
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	Powers\icearmor.bhvr
		Part	Powers\FrostSmall.part
		LifeSpan 17
	End

	Event
		Type	Local
		At	LlegR
		Bhvr 	Powers\icearmor.bhvr
		Part	Powers\FrostSmall.part
		LifeSpan 17
	End
		Event
		Type	Local
		At	LlegL
		Bhvr 	Powers\icearmor.bhvr
		Part	Powers\FrostSmall.part
		LifeSpan 17
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   Powers\Snowfall.part
		Part	Powers\FrostSmall.part
		LifeSpan 13
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   Powers\Snowfall.part
		Part	Powers\FrostSmall.part
		LifeSpan 13
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	5
	RepeatJitter 5

	Event
		Type	Local
		At	CHEST
		ChildFx	V_COV\PhysicsEnabled\IceChunkScatterSmallNonJagged.fx
		LifeSpan 120
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound aim2 60 60 .6
	End

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	Powers\YellowUpGlow.part
		Part2	Powers\YellowUpSpecks2.part
		Lifespan 13
	End
End

Condition
	On	Time
	Time	10

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	Powers\YellowUpRing01.part
		Part3	Powers\YellowUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	Powers\YellowUpRing02.part
		Part4	Powers\YellowUpRing02a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	Powers\YellowUpRingSlow.part
		Part3	Powers\YellowUpRing03.part
		Part4	Powers\YellowUpRing03a.part
	End
End

#############################################################

End