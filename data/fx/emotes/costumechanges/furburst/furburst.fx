#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type Local
		At Root
		Sound AnimalPack_FurBurst 100 100 1
	End

	Event
		EName	ChestAnchor
		Type	Start
		At	Chest
		ChildFX	:Body_Fur.fx

		LifeSpan 	1
	End
End

Condition
	On	Time
	Time	3
	Event
		EName	ChestAnchor
		Type	Start
		At	Chest
		Part	:DustDevil_Dust_Expanding.part
		Part	:DustDevil_Dust_Ground.part
		Part	:Dust_Flash.part
		LifeSpan 	5

	End

End

#############################################################

End