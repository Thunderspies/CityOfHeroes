#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################


Condition
	On	Cycle
	Time	15
	Chance	0.4
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
		At	WepL
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
End

#############################################################

End