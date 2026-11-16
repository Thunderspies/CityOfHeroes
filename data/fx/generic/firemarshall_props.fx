#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End


##################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Group
		Type	Start
		At	Root 
		Geom	Prop_Waterbackpack_group
		Bhvr	Behaviors/FireMarshallPropGroup.bhvr
	End

	Event
		EName 	Single
		Type	Start
		At	Root 
		Geom	Prop_Waterbackpack
		Bhvr	Behaviors/FireMarshallPropSingle.bhvr
	End


End

Condition
	On	Death

	Event
		Type	Local 
		At	Root 
		LifeSpan 360
	End
End

End
##################################

