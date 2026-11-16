#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:FadeToBlack_7sec.part
		Lifespan 300
	End
End

#############################################################

End
