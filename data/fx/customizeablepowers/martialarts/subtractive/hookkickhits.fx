#########################################################
##	Hook Kick Hit FX
########################################################

FxInfo
Lifespan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	:MAHitStar01.part
		Part2	:MAHitStar02.part
		Part3	:MAHitStar03.part
		Part4	:MAHitGlow01.part
		Part5	:MAHitRays01.part
		Sound Hit5 100.0 100.0 .92

	End

End

########################################################

End
