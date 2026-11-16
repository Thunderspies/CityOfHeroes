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
		Part1	CustomizeablePowers\Pets\StoneGolem\superStrengthDust1.part
		part2	CustomizeablePowers\Pets\StoneGolem\superStrengthDust2.part
		Part3	CustomizeablePowers\Pets\StoneGolem\WeaponRocks05.part
		Part4	CustomizeablePowers\Pets\StoneGolem\WeaponRocks05.part
		Sound Groundburst 80 80 0.96
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	ChestDust
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\Pets\StoneGolem\StalagtiteDustHit.part
		Part2	CustomizeablePowers\Pets\StoneGolem\StalagtiteDustHit2.part
		Lifespan 50
	End
End

#############################################################

End
