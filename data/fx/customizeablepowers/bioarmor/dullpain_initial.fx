#########################################################
## Invulnerability - Dull Pain Initial FX
#########################################################

FxInfo

Lifespan 60

#########################################################

Condition
	On	Time
	Time	5

	Event
		Ename	BoostMist
		Type	Local
		At	Hips
		Part2	POWERS\Invulnerability\DullPain_GreenMist.part
		#Sound regen8c 60 60 .76
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
		Part1	POWERS\Invulnerability\DullPain_Ring01.part
		LifeSpan	60
	End
End

#########################################################

End