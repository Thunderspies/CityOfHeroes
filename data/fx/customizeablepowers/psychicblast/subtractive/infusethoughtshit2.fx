#############################################################
## InfuseThoughtsHit2.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	faceringsRigging
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:InfuseRings.part
		Part4	:PercentSign.part
		Part5	:Asterick.part
	End

	Event
		EName	facerings
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:Defuseglow.part
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
	End

	Event
		EName	facerings2
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:PsionicBubblesHit.part
		Part4	:HitStreaksBig.part
		Lifespan 25
	End
End

#############################################################

End