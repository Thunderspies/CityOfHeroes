#############################################################
## FX_Flyer_Engine.fx
#############################################################

FxInfo
Flags  InheritAlpha   InheritAnimScale

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ArachnosFlyerTurbine_Loop 80 80 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At 	Root
		BhvrOverride
			Scale	3.0 3.0 3.0
		End
		Part	:FX_Flyer_lift.part
		Part	:FX_Flyer_fandark.part
	End


	Event
		Type	Local
		At 	Root
		BhvrOverride
			Scale	8.0 3.0 8.0
		End
		Part	:FX_Flyer_lift.part
	End
End

#############################################################

End

