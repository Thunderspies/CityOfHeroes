#########################################################
## Black Dwarf Strike
#########################################################

FxInfo

LifeSpan	50

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local 
		At	Origin
		sound	peacepunch 60 50 .87
	End
End

## HANDS #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part	:PunchTendrils.part
		Part	:PunchMistEnergy.part
		Part	:PunchTendrilsBlue.part
		Part	:PunchMistEnergyBlue.part
		Part	:PunchTendrilsPurple.part
		Part	:PunchMistEnergyPurple.part
		LifeSpan 25
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part	:PunchTendrils.part
		Part	:PunchMistEnergy.part
		Part	:PunchTendrilsBlue.part
		Part	:PunchMistEnergyBlue.part
		Part	:PunchTendrilsPurple.part
		Part	:PunchMistEnergyPurple.part
		LifeSpan 25
	End
End

#########################################################

End			