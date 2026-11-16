#############################################################
## EarthHitContinuing.fx
#############################################################

FxInfo
LifeSpan 30
ClampMaxScale 3.5 3.5 3.5

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:EarthRocksA.part
		Part2	:EarthMistA.part
		Part2	:EarthMistB.part
	End
End

#############################################################

End