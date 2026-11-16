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
		BHVR	behaviors\floating_fire_offset.bhvr
		Part	:Opposite90WindowFireFlame_LargeA.part
		Part	:Opposite90WindowFireFlame_LargeB.part
		Part	:Opposite90Window_SmokeLarge.part
		Part	:Opposite90Window_EmbersLarge.part
		Sound torch2_loop 37 37 .33
	End
End

End			