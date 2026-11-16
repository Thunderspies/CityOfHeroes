#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	50

Input  
	InpName	head
End

###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	faceringsRigging
		Type	local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PsionicAssault\InfuseLight2.part
		Part2	CustomizeablePowers\PsionicAssault\Infuseglow.part
		Part3	CustomizeablePowers\PsionicAssault\InfuseRings.part
		Part4	CustomizeablePowers\PsionicAssault\PercentSign.part
		Part5	CustomizeablePowers\PsionicAssault\Asterick.part
		Sound Infuse4 80 80 .6
	End

	Event
		EName	facerings
		Type	local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		
		Part1	CustomizeablePowers\PsionicAssault\InfuseLight2.part
		Part2	CustomizeablePowers\PsionicAssault\Infuseglow.part
		Part3	CustomizeablePowers\PsionicAssault\Defuseglow.part
		Part4	CustomizeablePowers\PsionicAssault\QuestionMark.part
		Part5	CustomizeablePowers\PsionicAssault\NumberSign.part
	End

	Event
		EName	facerings2
		Type	local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		
		Part1	CustomizeablePowers\PsionicAssault\PsionicBubblesHit.part
		Part2	CustomizeablePowers\PsionicAssault\HitRaysBig.part
		Part4	CustomizeablePowers\PsionicAssault\HitStreaksBig.part
	End

			
End

Condition
	On Time
	Time 25

	Event
		EName	facerings2
		Type	Destroy
	End

End

Condition
	On Time
	Time 30

	Event
		EName	All
		Type	Destroy
	End
End


End			