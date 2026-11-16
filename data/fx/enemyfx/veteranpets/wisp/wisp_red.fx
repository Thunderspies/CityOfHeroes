#############################################################
## Veteran Reward Combat Pet - Red Wips
#############################################################

FxInfo

Flags InheritAlpha

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset_Spin_01
		Type	Local
		At	Hips
		Bhvr	:Offset_Spin_01.bhvr
		#Sound	Whisp1_loop 120 80 .7
	End

	Event
		EName	Offset_Spin_02
		Type	Local
		At	Offset_Spin_01
		Bhvr	:Offset_Spin_02.bhvr
	End

	Event
		Type	Local
		At	Offset_Spin_02
		part	:Red_BodyGlow_01.part
		part	:Red_SwirlGlow_01.part
		part	:Red_Spark_01.part
	End
End

#############################################################

End