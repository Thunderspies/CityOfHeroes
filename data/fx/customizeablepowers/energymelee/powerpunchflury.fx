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
		Sound powerup2 100 100 .8
	End
End

## HANDS #######################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Largepowerpunch2.part
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchGlow.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchRings.part
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
		Part1	CustomizeablePowers\EnergyMelee\HitRingFlat.part
		Part2	CustomizeablePowers\EnergyMelee\HitRingThinFlat.part
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