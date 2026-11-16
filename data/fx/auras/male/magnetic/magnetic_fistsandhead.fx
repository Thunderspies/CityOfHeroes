#############################################################
## Magnetic_Fists.fx
#############################################################

FxInfo
Flags DontSuppress

########################################################



## SOUND FX #######################################################


Condition
	On	Time
	Time	1
	Repeat  4

	Event
		EName	Geo
		Type	Local
		At	Head
		ChildFX	:CHILD_Magnetic_3DDebris_Fist.fx

	End

End

Condition
	On	Time
	Time	1
	Repeat  4

	Event
		EName	Geo
		Type	Local
		At	HandL
		ChildFX	:CHILD_Magnetic_3DDebris_Fist.fx

	End

End

Condition
	On	Time
	Time	1
	Repeat  4

	Event
		EName	Geo
		Type	Local
		At	HandR
		ChildFX	:CHILD_Magnetic_3DDebris_Fist.fx

	End

End

########################################################

End