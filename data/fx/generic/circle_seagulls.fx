#############################################################
## FX_Seagull_Ultimate.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################

Condition
	On Cycle
	Time 120
	Chance .25
	Event
		Type Local
		At	Root
		Sound seagull1 550 500 .5
		Sound seagull2 550 500 .5
		Sound seagull3 550 500 .5
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	50


	Event
		EName 	Birds
		Type	Local
		At	Origin
		CHILDFX	:FX_Seagull_Ultimate.fx
		Lifespan 600
	End

End

#############################################################

End