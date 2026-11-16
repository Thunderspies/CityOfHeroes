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
		Part	:Blackout.part
		Part	:Blackout_Edges.part
	End
End

#############################################################

End
