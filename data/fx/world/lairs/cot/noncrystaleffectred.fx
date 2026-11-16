#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	root
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	P2
		Type	Local 
		At	root
		Part1	:CrystalMist2Red.part
		Part2	:CrystalMist1.part
		Part3	:CrystalSparksRed.part
		Part5	:CrystalSparksRed.part
		Part4	:CrystalSparksRed.part
		Part5	:CrystalSparksRed2.part
		Part4	:CrystalSparksRed2.part
		Part4	:CrystalSparksRed2.part
	End

	Event
		EName 	Pgg
		Type	Local 
		At	root
		Part1	:GhostsFaint.part
		
	End

End


End			