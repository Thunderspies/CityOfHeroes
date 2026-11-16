#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CameraSpace
		Part	:Redout.part
		Part	:Redout_Edges.part
	End
End

#############################################################

End