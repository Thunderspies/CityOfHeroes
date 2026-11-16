#########################################################
##	Storm Kick Hit FX
########################################################
FxInfo

Input  
	Inpname	Chest
End

Input  
	Inpname	Head
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	UarmR
End

Lifespan 100

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	Posit 
		At	Chest
		Part1	:MAHitStar01Sml.part
		Part2	:MAHitStar02Sml.part
		Part3	:MAHitStar03Sml.part
		Part4	:MAHitGlow01Sml.part
		Part5	:MAHitRays01Sml.part
		Sound Hit3 100.0 100.0 .66
		
	End

End


End
