#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	:Fireworks_Eyes.fx
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.5
	Random	1


	Event
		Type	Local
		At	WepL
		BhvrOverride
			StartJitter	0.35 0.35 0.35
		End
		ChildFX	:Fireworks_Tendril_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	WepR
		BhvrOverride
			StartJitter	0.35 0.35 0.35
		End
		ChildFX	:Fireworks_Tendril_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			StartJitter	0.35 0.35 0.35
		End
		ChildFX	:Fireworks_Tendril2_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	WepR
		BhvrOverride
			StartJitter	0.35 0.35 0.35
		End
		ChildFX	:Fireworks_Tendril2_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			StartJitter	0.35 0.35 0.35
		End
		ChildFX	:Fireworks_Tendril3_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	WepR
		BhvrOverride
			StartJitter	0.35 0.35 0.35
		End
		ChildFX	:Fireworks_Tendril3_Small.fx
		Lifespan 90
	End
End

#############################################################

End