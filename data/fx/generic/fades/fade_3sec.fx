#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo
Lifespan 210

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:FadeToBlack_3sec.part
		Lifespan 210
	End
End

#############################################################

End
