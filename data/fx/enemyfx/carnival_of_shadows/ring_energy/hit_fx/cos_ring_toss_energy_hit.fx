#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	120

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName 	HitFX
		Type	local 
		At	Chest
		Part	:Ring_HitFX_Main.part
		Part	:Ring_HitFX_Main3.part
		Part	:Ring_HitFX_Main2.part
		Part1	:MAHitStar01Sml.part
		Part2	:MAHitStar02Sml.part
		Part3	:MAHitStar03Sml.part
		Part4	:MAHitGlow01Sml.part
		Part5	:MAHitRays01Sml.part
		Sound EnergyHit2 60 60 .72
		LifeSpan	10
		
		
	End

End
