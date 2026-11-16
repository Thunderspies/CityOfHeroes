#########################################################
## Mitochondria - Endocronic Blast
#########################################################

FxInfo

LifeSpan	60

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	PreFire
		Type	Local 
		At	Waist
		Part1	:FireHandz.part
		LifeSpan	15
	End
End

Condition
	On	Time
	Time	16
	Event
		Type 	Local
		At 	Origin
		Sound mitoblast1 80 80 .88
	End
End

#########################################################

End	