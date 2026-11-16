#############################################################
## Icon_MissionInProgress.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 7.5 -0.5 #1 2 0
			PYRRotate	0 0 0
			Scale		1.5 1.5 1.5
		End
		GEOM	Icons_Contact_MissionCompleted
	End
End

#############################################################

End