#########################################################
## Slime Aura Head
#########################################################

FxInfo

Flags DontSuppress
########################################################



## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
#		Sound explohit 80 80 0.77
	End
End



Condition
	On	Time
	Time	1
	Repeat  10

	Event
		EName	Geo
		Type	PositOnly
		At	Chest
		ChildFX	:CHILD_Magnetic_3DDebris.fx

	End

End

End