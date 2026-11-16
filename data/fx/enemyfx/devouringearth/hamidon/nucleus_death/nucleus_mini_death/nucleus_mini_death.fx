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

#########################################################

LifeSpan 40


Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_01.part
		Part	:Hamidon_Mito_Attack_Burst_Flat_01.part
		Sound mitohit1 60 60 .7
		Lifespan	3
	End
End

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_Glow_01.part
		Lifespan	3
	End
End

Condition
	On 	Time
	Time 	5	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_Bits_01.part
		Part	:Hamidon_Mito_Attack_Burst_Bits_02.part
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
		Lifespan	40
	End
End

