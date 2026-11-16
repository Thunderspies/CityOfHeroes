#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
End
 

Flags    InheritAlpha


########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_01.part
		Bhvr	:Offset_Eye_R_01.bhvr
		Sound TarantulaEyes_loop 50 50 .36
	End
End
Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_02.part
		Bhvr	:Offset_Eye_R_02.bhvr
	End
End
Condition
	On 	Time
	Time 	20

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_03.part
		Bhvr	:Offset_Eye_R_03.bhvr
	End
End
Condition
	On 	Time
	Time 	30

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_04.part
		Bhvr	:Offset_Eye_R_04.bhvr
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_01.part
		Bhvr	:Offset_Eye_L_01.bhvr
	End
End
Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_02.part
		Bhvr	:Offset_Eye_L_02.bhvr
	End
End
Condition
	On 	Time
	Time 	20

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_03.part
		Bhvr	:Offset_Eye_L_03.bhvr
	End
End
Condition
	On 	Time
	Time 	30

	Event
		Type	Local 
		At	Hips
		Part1	:Toxic_Glow_Eye_04.part
		Bhvr	:Offset_Eye_L_04.bhvr
	End

End

End
##################################

