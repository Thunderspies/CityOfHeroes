#
FxInfo


###########################################################

Condition
	On Time
	time 0

	Event
		Ename ground
		At Root
		Type Local
		Part :Roach_Trail.part
	End
End

	Condition
		On Cycle
		Time 5
		Random 1
		Chance 0.8

		Event
			At Chest
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At WepL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At WepR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LLegL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LLegR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At UArmL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At UArmR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LArmL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LArmR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

	End

End
