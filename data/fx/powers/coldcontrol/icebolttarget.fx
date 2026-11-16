
#########################################################
##	iced target
##
FxInfo  

Input  
	InpName	Origin
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		Part1	:icebolttargetfreeze.part
		Part2	:Generic_hitflash.part
		Part3	:Generic_hitflash2.part
		Part4	:iceboltsnowexp.part
	End

End


Condition
	On 	Time
	Time 	100

	Event
		EName 	All
		Type	Destroy 
	End
End


End			