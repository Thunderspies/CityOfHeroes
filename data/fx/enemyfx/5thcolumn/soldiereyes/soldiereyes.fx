#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
End

Input  
	Inpname	Head
End


Flags    InheritAlpha
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Head
		Geom	FX_night
		#Bhvr	Behaviors/5thEyeScale2.bhvr
	End

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		Part1	EnemyFx/5thColumn/Requiem/RequiemEyes.part
		Part2	EnemyFx/5thColumn/Requiem/RequiemPupil.part
		
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	EnemyFx/5thColumn/Requiem/RequiemEyes.part
		Part2	EnemyFx/5thColumn/Requiem/RequiemPupil.part
		
	End

End

Condition
	On		TriggerBits
	TriggerBits	DEATH HIDE

	Event
		Type	Destroy
		EName	All
	End
End

End
##################################

