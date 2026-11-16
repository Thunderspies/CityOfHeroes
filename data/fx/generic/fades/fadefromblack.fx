#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CameraSpace
		Part	:FadeFromBlack.part
	End
End

#############################################################

End