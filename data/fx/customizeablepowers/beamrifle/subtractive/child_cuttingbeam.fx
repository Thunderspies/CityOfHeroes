#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 20

#############################################################

Condition
	On	Cycle
	Time	1

	Event
		Type	Start
		At	WepL
		Part	:Beam_Small_AfterGlow.part
		PMagnet	BeamEnd
		LookAt	BeamEnd
		Pother	BeamEnd
		Lifespan 12
	End
End

#############################################################

End