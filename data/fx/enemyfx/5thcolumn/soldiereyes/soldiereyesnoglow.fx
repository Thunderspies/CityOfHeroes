#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	head
End

Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	head
		Bhvr	Behaviors/5thEyeScale.bhvr
		Geom	FX_Vamp
	
	End

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		#Part1	EnemyFx/5thColumn/Requiem/RequiemEyes.part
		Part2	EnemyFx/5thColumn/Requiem/RequiemPupil.part
		
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		#Part1	EnemyFx/5thColumn/Requiem/RequiemEyes.part
		Part2	EnemyFx/5thColumn/Requiem/RequiemPupil.part
		
	End

	
End

End
##################################

