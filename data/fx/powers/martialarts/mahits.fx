#########################################################
##	Martial Arts Hit FX
########################################################
FxInfo

Lifespan 60

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
		Part1	:MAHitStar01.part
		Part2	:MAHitStar02.part
		Part3	:MAHitStar03.part
		Part4	:MAHitGlow01.part
		Part5	:MAHitRays01.part
		Sound Hit3 100.0 100.0 .8
		
	End

End


End
