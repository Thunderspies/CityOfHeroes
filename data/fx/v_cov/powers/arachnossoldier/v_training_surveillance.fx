#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale DontSuppress

Lifespan 105

## SOUNDS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Sound Training_Surveillance_Activate_01 120 120 0.75
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	:PowerAnalyzerOffset_01.bhvr
		BhvrOverride
			StartColor	255 16 0
		End
		Geom	GEO_PowerAnalyzer_01
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	:PowerAnalyzerOffset_01.bhvr
		BhvrOverride
			StartColor	255 64 0
		End
		Geom	GEO_PowerAnalyzer_02
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	:PowerAnalyzerOffset_01.bhvr
		BhvrOverride
			StartColor	255 0 0
		End
		Geom	GEO_PowerAnalyzer_03
	End
End

##############################################################

End