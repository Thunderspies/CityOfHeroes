#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Back
End

Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Nemesis
		Type	local
		At	Back
		Geom	NemesisBackPac
		BhvrOverride
			StartColor	130 130 130
		End
		Sound smoke2_loop 50 50 .5
	
	End
	
	Event
		EName 	NemesisSmoke1
		Type	local 
		At	Nemesis
		 AltPiv 1
		ChildFx	EnemyFx/Nemesis/smoke.fx
		Magnet	Nemesis
				
	End
	
	Event
		EName 	NemesisSmoke2
		Type	local 
		At	Nemesis
		 AltPiv 2
		ChildFx	EnemyFx/Nemesis/smoke.fx
		Magnet	Nemesis		
	End

End	

Condition
	On 	TriggerBits
	TriggerBits	Death

	Event
		EName 	NemesisSmoke1
		Type	Destroy
	
	End

	Event
		EName 	NemesisSmoke2
		Type	Destroy
	
	End

End

End
##################################

