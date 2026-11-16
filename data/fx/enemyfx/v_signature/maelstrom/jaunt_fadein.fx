#############################################################
## Jaunt.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest
		Sound vandalblast2 80 80 0.8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	MaelstromJaunt
		Type	Start
		At	Chest
		Bhvr	:FadeIn.bhvr
		Part4	:Jaunt_Glow.part

		Lifespan 1
	End

	Event
		EName 	MaelstromJaunt
		Type	StartPositOnly
		At	Chest
		Part2	:Jaunt_Flash.part
		Lifespan 1
	End
End

Condition
	On	TIme
	Time	3

	Event
		EName 	MaelstromJaunt
		Type	StartPositOnly
		At	Chest
		Part	:Jaunt_Hurricane_Lingering.part
		Part	:Jaunt_Hurricane.part
		Lifespan 1
	End
End
#############################################################

End