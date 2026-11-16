#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Mystic
End


LifeSpan	100

Condition
	On 	Time
	Time 	26

	Event
		EName 	Mallet
		Type	Local 
		At	mystic
		Part1	:StalagtiteDustHit.part
		Part2	:StalagtiteDustHit2.part
		Part3	:WeaponRocks05.part
		Part4	:WeaponRocks05.part
	End

End


End
