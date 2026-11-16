#########################################################
## Darkness Control - Darkest Night (Continuing FX)
#########################################################

FxInfo

## CONTINUING FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hit
		Type	local
		At	chest
		part1	CustomizeablePowers\DarkMiasma\DamageHitMist.part
		part2	CustomizeablePowers\DarkMiasma\DefenceHitMist2.part
		Part	CustomizeablePowers\DarkMiasma\ChillOfNightTendril.part
	End
End

#########################################################

End