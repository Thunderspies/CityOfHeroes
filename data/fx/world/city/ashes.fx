#############################################################
## AbyssalMending_Hit.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

#	Event
#		Type	Start
#		At	Root
#		part	:Ashes_TEST.part
#	End

	Event
		EName	AshAnchor
		Type	Start
		CAMERASPACE
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 25.0 0.0
		End
		Part	:Ashes.part
	End
End

#############################################################

End


