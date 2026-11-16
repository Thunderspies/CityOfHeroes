#############################################################
## Child_CountdownFlash_Slow.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	7.5

	Event
		Type	PositOnly
		At	Emblem
		Part	:CountdownFlash_Fast1.part
		Lifespan 2
	End
End

#############################################################

End