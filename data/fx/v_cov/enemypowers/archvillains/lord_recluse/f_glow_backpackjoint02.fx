
##############################################################################################
######   
######  Glow at First joint of backpack arms 
######  
##############################################################################################

FxInfo

Condition
	On	Time
	Time	0

	Event
		EName	Child_CB1_02
		Type	Local
		At	C_Fore_FootL
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End

	Event
		EName	Child_CB2_02
		Type	Local
		At	C_Fore_FootR
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End

	Event
		EName	Child_CB3_02
		Type	Local
		At	C_FootL
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End

	Event
		EName	Child_CB4_02
		Type	Local
		At	C_FootR
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End

	Event
		EName	Child_CB5_02
		Type	Local
		At	C_HandL
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End

	Event
		EName	Child_CB6_02
		Type	Local
		At	C_HandR
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End

		Event
		EName	Child_CB7_02
		Type	Local
		At	C_Hind_FootL
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End

	Event
		EName	Child_CB8_02
		Type	Local
		At	C_Hind_FootR
		ChildFX :F_ChildGlow_RecluseRedKnuckles.fx
	End
End



End