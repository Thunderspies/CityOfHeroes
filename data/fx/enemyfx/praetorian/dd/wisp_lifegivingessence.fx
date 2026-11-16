#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################

Condition
	On 	Time
	Time 	0	

##	Event
##		EName	Offset_Spin_01
##		Type	Local
##		At	Chest
##		Bhvr	:Wisp_Offset_Spin_01.bhvr
##		Sound Whisp1_loop 120 120 .7
##	End
##
##	Event
##		EName	Offset_Spin_02
##		Type	Local
##		At	Offset_Spin_01
##		Bhvr	:Wisp_Offset_Spin_02.bhvr
##	End

	Event
		EName	1
		Type	Local
		At	Head
##		At	Offset_Spin_02
		part	:Wisp_Body_Glow_01.part
	End

	Event
		EName	2
		Type	Local
		At	Head
##		At	Offset_Spin_02
		part	:Wisp_Swirl_Glow_01.part
	End

	Event
		EName	3
		Type	Local
		At	Head
##		At	Offset_Spin_02
		part	:Wisp_Spark_01.part
	End

	Event
		EName 	4
		Type	Local 
		At	Head
		Altpiv	1
		Part	:Wisp_Cloud1.part

	End

	Event
		EName 	5
		Type	Local 
		At	Head
		Altpiv	1
		Part	:Wisp_Cloud2.part
	End

End
	
End
