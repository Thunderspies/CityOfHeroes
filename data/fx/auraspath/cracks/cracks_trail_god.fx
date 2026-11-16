#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		Time #Cycle
	Time     	0
#	DoMany		1

	Event
		ENAME	GODTrail
		Type	Local
		At	Root
		Flags	OneAtATime
		ChildFX	:Cracks_Trail.fx

	End
End

#########################################################

End