#############################################################
## Good.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	:LightGlow.part
		Part	:Lightbeams.part
		Part	:LightBeams_Chest.part
		POther	Chest
	End
End

#############################################################

#Condition
#	On	Cycle
#	Time	30
#
#	Event
#		Type	SetLight
#		BHVR	:LightPulse_Light.bhvr
#		LifeSpan 300
#	End
#End

#############################################################

End