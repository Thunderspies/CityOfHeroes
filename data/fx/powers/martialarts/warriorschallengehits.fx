#########################################################
##	Martial Arts Hit FX
########################################################
FxInfo

Input  
	Inpname	Head
End


Condition
	On 	Time
	Time 	20

	Event
		EName 	HitFX
		Type	local 
		At	Head
		Part1	:MAWarrChallHit01.part
		Part2	:MAWarrChallHitRays01.part
		Sound Hit3 100.0 100.0 .8
		
	End

End


End
