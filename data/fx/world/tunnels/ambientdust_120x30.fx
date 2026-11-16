#############################################################
## AmbientDust.fx
#############################################################

FxInfo
#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	AshAnchor
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:Ambient_Dust_120x30.part
	End
End
#############################################################
End

