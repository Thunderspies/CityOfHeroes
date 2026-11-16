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

Input  
	Inpname	Waist
End

Flags    InheritAlpha

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound hamidonsplat 50 50 .4
	End
End



Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Devouring_Mini_Hamidon_Glow3.part
		Part	:Devouring_Mini_Hamidon_Glow1.part
		Part	:Devouring_Mini_Hamidon_Glow2.part
		Part	:Devouring_Mini_Hamidon_Cloud_01.part
		Sound Hamidon2_loop 44 44 .6
	End
	

	Event
		EName 	Hamidon
		Type	Local
		At	Hips
		Geom	Hamidon_01_Mini

	End
End
