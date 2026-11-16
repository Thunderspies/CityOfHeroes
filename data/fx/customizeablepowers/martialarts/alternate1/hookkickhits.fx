#########################################################
##	Hook Kick Hit FX
########################################################
FxInfo

Input  
	Inpname	Chest
End

Lifespan 100

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	local 
		At	Chest
		Part1	CustomizeablePowers\MartialArts\MAHitStar01.part
		Part2	CustomizeablePowers\MartialArts\MAHitStar02.part
		Part3	CustomizeablePowers\MartialArts\MAHitStar03.part
		Part4	CustomizeablePowers\MartialArts\MAHitGlow01.part
		Part5	CustomizeablePowers\MartialArts\MAHitRays01.part
		Sound Hit5 100.0 100.0 .92
		
	End

End


End
