#########################################################
##	template

FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Hips
End

Input  
	Inpname	Head
End

Input  
	Inpname	Eyes
End

Input  
	Inpname	Root
End

Input  
	Inpname	Chest
End

Flags    InheritAlpha

#########################################################


Lifespan	30

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Hydra_Pod_Node
		Type	local
		At	Hips
		Geom	Hydra_Regeneration_Pod_Node
	End

	Event
		EName 	Hamidon
		Type	local
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Exit_Explosion.part
		Part	:Hydra_Pod_Exit_Beams.part
		Part	:Hydra_Pod_Exit_Cloud.part
		Part	:Hydra_Pod_Exit_Explosion_02.part
		Part	:Hydra_Pod_Exit_Explosion_03.part
		Part	:Hydra_Pod_Exit_Beams_Thin.part
	End	


End
