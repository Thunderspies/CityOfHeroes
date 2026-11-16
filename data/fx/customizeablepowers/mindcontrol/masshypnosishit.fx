#############################################################
## MassHypnosisHit.fx
#############################################################

FxInfo
Lifespan 100

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part	:HeadDizzies.part
		Part	:HeadDizzyRings.part
		Part	:MindGlowHit2.part
		Sound mentalhit 80 80 0.8
	End
End

#############################################################

End