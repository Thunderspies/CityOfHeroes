#########################################################
## Slime Aura Head
#########################################################

FxInfo
Flags DontSuppress

########################################################



## SOUND FX #######################################################


Condition
	On	Time
	Time	1
	Repeat  6

	Event
		EName	Geo
		Type	Local
		At	HandL
		ChildFX	:CHILD_Magnetic_3DDebris_Fist.fx

	End

End

########################################################

End