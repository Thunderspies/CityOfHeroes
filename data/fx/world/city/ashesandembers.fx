#############################################################
## AbyssalMending_Hit.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	AshAnchor
		Type	Start
		CAMERASPACE
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 25.0 0.0
		End
		Part	:Ashes_Quick.part
		Part	:Embers.part
		Part	:Dust.part
	End
End

#############################################################

End


