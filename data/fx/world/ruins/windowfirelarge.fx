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

#	Event
#		EName 	geometerydummy
#		Type	Local 
#		At	Root
#		geom	centerdummy
#		Bhvr	:Offset90.bhvr
#	End


	Event
		EName 	Prime
		Type	local 
		At	chest
		BHVR	:OffsetDown.bhvr
		Part	:WindowFireFlame_LargeA.part
		Part	:WindowFireFlame_LargeB.part
		Part	:Window_SmokeLarge.part
		Part	:Window_EmbersLarge.part
		Sound torch2_loop 37 37 .33
	End
End

End			