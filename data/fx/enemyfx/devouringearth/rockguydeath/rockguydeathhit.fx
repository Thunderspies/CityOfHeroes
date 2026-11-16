#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	Local 
		At	Chest
		Part1	:StalagtiteDustHit.part
		Part2	:StalagtiteDustHit2.part
		Part3	:WeaponRocks06.part
		Part4	:BoulderBreak.part
		Part4	:BoulderBreak2.part
		Part5	:WeaponRocks07.part
		Sound stonehit2 100 100 .85

	End


End


End
