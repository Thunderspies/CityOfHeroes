#########################################################
##	cot_waterfall_med.fx
##
FxInfo


Input  
	InpName	head
End

###########################################################




Condition
	On	Time
	Time	0

#	Event
#		Type	local
#		At	Waist
#		
#		Part	:HoldRings.part
#		Part	:HoldRings2.part
#		Part	:GlowHoldRings.part
#		Part	:GlowHoldRings2.part
#
#	End

	Event
		Type	local
		At	Root
		BHVR	Behaviors/RootToKneesAdjustment.bhvr
		Part	:HoldRings.part
		Part	:HoldRings2.part
		Part	:GlowHoldRings.part
		Part	:GlowHoldRings2.part
		Part	:WhiteFire_flame.part
		Sound Fettering_loop 60.0 60.0 .36

	End
End

End			