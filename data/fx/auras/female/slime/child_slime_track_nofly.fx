#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		Time #Cycle
	Time	    	0
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:GRANDCHILD_Slime_Trail.fx
	End
End


#########################################################

End