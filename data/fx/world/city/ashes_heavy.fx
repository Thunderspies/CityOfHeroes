#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	AshAnchor
		Type	StartPositOnly
		CAMERASPACE
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 25.0 0.0
		End
		Part	:Ashes_LongLife.part
		Part	:Ash_Cloud.part

	End
End

#############################################################

End


