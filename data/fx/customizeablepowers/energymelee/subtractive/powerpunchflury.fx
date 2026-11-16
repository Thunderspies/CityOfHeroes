#############################################################
## PowerPunchFlury.fx
#############################################################

FxInfo

Lifespan 60

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Sound powerup2 100 100 0.8
	End
End

## HANDS #######################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch2.part
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchGlow.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchRings.part
		Lifespan 15
	End
End

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	start
		At	mystic
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\HitRingFlat.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\HitRingThinFlat.part
		Lifespan 5
	End
End

## PHYSICS FORCES #######################################################

Condition
	On 	Time
	Time 	36

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		800
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#########################################################

End