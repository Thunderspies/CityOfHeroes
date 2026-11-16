#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Root
End

Input  
	Inpname	Mystic
End

LifeSpan 360
##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Local
		At	Mystic
		Geom	Prop_speaker
		LifeSpan 12	
	End

End


Condition
	On	Time
	Time	0
	Event
		EName 	Prop_Toss
		Type	Start
		At	Root 
		Anim	Prop_basic
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
	On	Time
	Time	13

	Event
		EName	Prop
		Type	Local
		At	Prop_Toss
		AnimPiv Hips
		Geom	Prop_speaker
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End


End
##################################

