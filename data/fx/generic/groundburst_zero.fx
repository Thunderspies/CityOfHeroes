#############################################################
## groundburst_zero.fx
#############################################################

FxInfo

Lifespan 60

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	GroundBurst
		Type	Local
		At	Mystic
		Part	POWERS\SuperStrength\superStrengthDust1.part
		Part	POWERS\SuperStrength\superStrengthDust2.part
		Part	POWERS\EarthControl\WeaponRocks05.part
		Part	POWERS\EarthControl\WeaponRocks05.part
		Sound Groundburst 80 80 .96
		Lifespan 39
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	ChestDust
		Type	Local
		At	Chest
		Part	POWERS\EarthControl\StalagtiteDustHit.part
		Part	POWERS\EarthControl\StalagtiteDustHit2.part
		Lifespan 47
	End
End

#############################################################

End