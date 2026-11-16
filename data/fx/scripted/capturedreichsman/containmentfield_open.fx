#############################################################
## CapturedReichsman.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SparksOffet1
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	7.5 0 0
			Scale		0.25 0.25 0.25
		End
		Geom	GEO_FX_TestSphere
	End
End

#############################################################

End