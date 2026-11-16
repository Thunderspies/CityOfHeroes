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
		Part1	:CrystalMist.part
		Part1	:CrystalMista.part
		Part2	:CrystalMist1.part
		Part3	:CrystalSparks1.part
		Part5	:CrystalSparks1.part
		Part4	:CrystalSparks1.part
		Part	:CrystalSparksInvert.part
		Part	:CrystalSparksInvert.part
		Part	:CrystalSparksInvert.part
	End

	Event
		EName 	Pgg
		Type	Local 
		At	root
		Part1	:GhostsFaint.part
		
	End

End

End			