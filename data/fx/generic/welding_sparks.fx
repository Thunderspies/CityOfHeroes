
FxInfo

Flags    InheritAlpha

########################################################
##Sparks
########################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest

		Sound welding_loop 45 45 .6

	End
End


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest

		Sound welding2_loop 44 44 .4

	End
End





Condition
	On 	Cycle
	Time 	10
	Chance	.45

	Event
		EName 	RArmSpark
		Type	Local
		At	WepR
		Part	:WeldingBlueSpark.part
		LifeSpan	10
		
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.55

	Event
		EName 	RArmSpark3
		Type	Local 
		At	WepR
		Part	:WeldingBlueGlow.part
		Part	:WeldingBlueGlowstar.part
		LifeSpan	3
	End

	Event
		EName 	RArmSpark6
		Type	Local 
		At	WepR
		Part	:WeldingBlueGlow.part
		Part	:WeldingBlueGlowstar.part
		LifeSpan	3
	End

End


End



				