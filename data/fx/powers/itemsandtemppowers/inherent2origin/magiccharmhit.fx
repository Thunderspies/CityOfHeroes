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
		At	Neck
		Sound magic_poof1 70 70 .89
	End

	Event
		EName 	Hit
		Type	Start
		At	Chest
		Part2	:DartHitflashStar.part
		Part1	WEAPONFX\Pistolflash.part
		Part	:MagicCharmFlash.part
		Part	:MagicCharmWisps.part
		Lifespan 5
	End
End

#############################################################

End