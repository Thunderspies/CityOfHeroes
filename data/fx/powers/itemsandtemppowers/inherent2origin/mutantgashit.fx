#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan  90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Sound mutagenexplo 70 70 .87
	End

	Event
		EName 	Hit
		Type	Start
		At	Chest
		Part2	:DartHitflashStar.part
		Part1	WEAPONFX\Pistolflash.part
		Part	:MutantGasWisps.part
		Part	:MutantGasCloud.part
		Lifespan 5
	End
End

#############################################################

End