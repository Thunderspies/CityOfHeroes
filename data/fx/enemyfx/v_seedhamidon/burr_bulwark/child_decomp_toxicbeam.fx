#############################################################
## Fault.fx
#############################################################

FxInfo


#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part	:Decomp_ToxicBeam.part
		Part	:Decomp_ToxicBeam2.part
		Lifespan 30
	End
End


#############################################################

End