
FxInfo

#####  Briefcase explode   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					
		Type	Local
		At	Root					
		Geom	Prop_Briefcase_01
		Geom	Prop_Briefcase_02
		Bhvr	Behaviors/briefcase_explode.bhvr
	End
		
End

End		