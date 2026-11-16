###########################################################

FxInfo

	Flags DontSuppress

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Fireball_Start 100 100 .8
	End
	Event
		Type	Local
		At	Root
		Sound 	Fireball_Loop 100 100 .4
	End

End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			Part :Fireball.part
			Part :Fireball2.part
		End

	End

###########################################################

End
