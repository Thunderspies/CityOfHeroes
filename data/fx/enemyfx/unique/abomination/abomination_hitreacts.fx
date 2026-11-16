#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 1

#############################################################
Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound WW_HitReact_01 200 200 .65
		Sound WW_HitReact_02 200 200 .65
		Sound WW_HitReact_03 200 200 .65
		Sound WW_HitReact_04 200 200 .65
		Sound WW_HitReact_05 200 200 .65
	End

End
