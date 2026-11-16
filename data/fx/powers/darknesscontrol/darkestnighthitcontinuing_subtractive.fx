#########################################################
## Darkness Control - Darkest Night (Continuing FX)
#########################################################

FxInfo

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hit
		Type	local 
		At	chest
		Sound Darknighthit 50 50 .88
	End
End

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
		Part	:DarkBlastHitPuddle2_Subtractive.Part
		Part	:ChillOfNightTendril.part
		Part	:DarkBlastHitTendril2_Subtractive.part
	End
End
	
#########################################################

End