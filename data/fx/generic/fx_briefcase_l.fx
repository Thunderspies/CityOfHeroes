#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	WepL
End




##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Local
		At	WepL 
		Geom	Prop_Briefcase_01
		Geom	Prop_Briefcase_02
		Geom	Prop_Briefcase_03
		Bhvr	Behaviors/briefcase_offset.bhvr
	End


End



End
##################################

