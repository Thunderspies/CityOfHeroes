#############################################################
## Icon_MissionInProgress.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On	Time
	Time	0

#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 7.5 -0.5
#			Scale		1.25 1.25 1.25
#		End
#		GEOM	Icon_Desdemona
#	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 7.5 -0.5
		End
		Part	:Icon_Desdemona.part
	End
End

#############################################################

End