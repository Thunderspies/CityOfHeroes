#########################################################
##	FireAura
FxInfo

LifeSpan	70

##################################


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Sound firering 60.0 60.0 .7


	End
End



Condition
	On	Time
	Time	8

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part
		Sound Firehand2 100.0 100.0 .45
		LifeSpan	70

	End

End


End