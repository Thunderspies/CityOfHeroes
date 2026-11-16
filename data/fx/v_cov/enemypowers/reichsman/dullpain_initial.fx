#########################################################
## Invulnerability - Dull Pain Initial FX
#########################################################

FxInfo

Lifespan 60

#########################################################

Condition
	On	Time
	Time	20

	Event
		Ename	BoostMist
		Type	Local
		At	Chest
		Part2	:DullPain_InitialBodyGlows.part
		Sound regen8c 60 60 .76
		LifeSpan	60
	End
End

Condition
	On	Time
	Time	58

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part1	:Ring.part
		LifeSpan	60
	End
End

#########################################################

End