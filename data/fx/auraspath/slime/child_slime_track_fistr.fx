#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS    	MALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Slime_Track_NOFLY_FistR.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS    	FEMALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Slime_Track_NOFLY_FistR.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS    	HUGE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Slime_Track_NOFLY_FistR.fx
	End
End


#########################################################

End