#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandL
End

Input  
	Inpname	Chest
End

Input  
	Inpname	Waist
End

LifeSpan	200

########################################################


######################################################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Mez_Core.part
		Part	:Hamidon_Mito_Mez_Twinkles.part
		Part	:Hamidon_Mito_Mez_Twinkles_02.part
		sound	mitoheal2 70 60 .7
		Lifespan	60
	End		
	
End


End