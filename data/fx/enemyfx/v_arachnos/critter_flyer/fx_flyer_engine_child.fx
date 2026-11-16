#############################################################
## FX_Flyer_Engine.fx
#############################################################

FxInfo
Flags  InheritAlpha   InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At 	HandL
		BhvrOverride
			PositionOffset -20 -2 -7
			Scale	3.0 3.0 3.0
		End
		Part	:FX_Flyer_lift.part
		Part	:FX_Flyer_fandark.part
	End


	Event
		Type	Local
		At 	HandL
		BhvrOverride
			PositionOffset -20 -2 -7
			Scale	8.0 3.0 8.0
		End
		Part	:FX_Flyer_lift.part
	End

	Event
		Type	Local
		At 	HandR
		BhvrOverride
			PositionOffset 20 -2 -7
			Scale	3.0 3.0 3.0
		End
		Part	:FX_Flyer_lift.part
		Part	:FX_Flyer_fandark.part
	End


	Event
		Type	Local
		At 	HandR
		BhvrOverride
			PositionOffset 20 -2 -7
			Scale	8.0 3.0 8.0
		End
		Part	:FX_Flyer_lift.part
	End
End

#############################################################

End

