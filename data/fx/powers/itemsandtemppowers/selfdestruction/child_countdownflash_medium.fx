#############################################################
## Child_CountdownFlash_Slow.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	15

	Event
		Type	PositOnly
		At	Emblem
		Part	:CountdownFlash_Medium1.part
		Lifespan 2
	End
End

#############################################################

End