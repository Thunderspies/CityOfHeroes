#############################################################
## Jaunt.fx
#############################################################

FxInfo

LifeSpan 30

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
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			FadeOutLength	5
		End
		Flags	AdoptParentEntity
		Part4	:Jaunt_Glow.part

		Lifespan 1
	End

	Event
		EName 	MaelstromJaunt
		Type	StartPositOnly
		At	Chest
		Part2	:Jaunt_Flash.part
		#part	:Jaunt_Ring.part

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