#############################################################
## Child_CountdownFlash_Slow.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	30

	Event
		Type	PositOnly
		At	Emblem
		Part	:CountdownFlash_Slow1.part
		Lifespan 2
	End
End

#############################################################

End