#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################

Condition
	On 	Time
	Time 	0	

	Event
		EName	Offset_Spin_01
		Type	Local
		At	Chest
		Bhvr	:Offset_Spin_01.bhvr
		Sound Whisp1_loop 120 120 .7

	End

	Event
		EName	Offset_Spin_02
		Type	Local
		At	Offset_Spin_01
		Bhvr	:Offset_Spin_02.bhvr

	End

	Event
		EName	1
		Type	Local
		At	Offset_Spin_02
		part	:Body_Glow_01.part
	End

	Event
		EName	2
		Type	Local
		At	Offset_Spin_02
		part	:Swirl_Glow_01.part
	End

	Event
		EName	3
		Type	Local
		At	Offset_Spin_02
		part	:Spark_01.part
	End

End
	
End
