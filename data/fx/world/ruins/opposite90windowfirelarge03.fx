#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Root
		#BHVR	:90WindowFireOffset.bhvr
		Part	:Opposite90WindowFireFlame_LargeA03.part
		#Part	:WindowFireFlame_LargeB03.part
		Part	:Opposite90Window_SmokeLarge2.part
		#Part	:Window_EmbersLarge03.part
		Sound torch2_loop 37 37 .33
	End
End

End			