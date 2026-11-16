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
		Sound 	CloudOfIce_Loop 100 100 .4
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			Part :shiverIce.part
			Part :ShiverMist.part
			Part :ShiverSnow.part
#			Part :snowBurst.part
#			Part :ColdArmMist.part
		End

	End

###########################################################

End
