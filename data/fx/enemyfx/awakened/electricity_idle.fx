#############################################################
## ElectricBody.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	45

	Event
		EName	Idle1
		Type	Local
		At	Root
		ChildFX	:CHILD_Electricity_Idle.fx
		SoundNoRepeat 1
		Sound FS_AwakenedElectricityIdle_01 50 50 .25
		Sound FS_AwakenedElectricityIdle_02 50 50 .25
		Sound FS_AwakenedElectricityIdle_03 50 50 .25
		Lifespan 38
	End
End

#############################################################

End