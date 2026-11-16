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

Input  
	Inpname	Waist
End

Flags    InheritAlpha

LifeSpan	200

#########################################################




Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_01.part
		Part	:Hamidon_Mito_Attack_Burst_Core.part
		Sound HamidonSplit2 70 70 .75
		Lifespan	10
	End
End

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_Bits_01.part
		Lifespan	7
	End
End

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_02.part
		Lifespan	10
	End
End
