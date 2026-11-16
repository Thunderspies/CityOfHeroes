#########################################################
##	Martial Arts Hit FX
########################################################
FxInfo

Input  
	Inpname	Chest
End


Condition
	On 	Time
	Time 	5

	Event
		EName 	HitFX
		Type	Start 
		At	Chest
		Part1	:MAHitStar01Sml.part
		Part2	:MAHitStar02Sml.part
		Part3	:MAHitStar03Sml.part
		Part4	:MAHitGlow01Sml.part
		Part5	:MAHitRays01Sml.part
		Sound Hit3 100.0 100.0 .8
		
	End

End


End
