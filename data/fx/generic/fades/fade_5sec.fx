#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo
Lifespan 240

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:FadeToBlack_5sec.part
		Lifespan 240
	End
End

#############################################################

End
