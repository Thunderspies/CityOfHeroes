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
		Part	:90WindowFireFlame_LargeA02.part
		Part	:90WindowFireFlame_LargeB02.part
		Part	:90Window_SmokeLarge.part
		#Part	:Window_EmbersLarge.part
		Sound torch2_loop 37 37 .33
	End
End

End			