#########################################################
## Darkness Control - Darkest Night (Continuing FX)
#########################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################


## CONTINUING FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hit
		Type	local
		At	chest
		part1	:DamageHitMist.part
		part2	:DefenceHitMist2.part
		Part	:ChillOfNightTendril.part
	End
End

#########################################################

End