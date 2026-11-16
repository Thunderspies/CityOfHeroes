#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part	:StreetTrash1.part
		Part	:StreetTrash2.part
		Part	:StreetLeaf01.part
		Part	:StreetLeaf02.part
		Part	:StreetLeaf03.part
		Part	:StreetLeaf04.part
		
	End

End
End			