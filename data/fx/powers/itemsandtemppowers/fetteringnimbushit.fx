#########################################################
##	cot_waterfall_med.fx
##
FxInfo


Input  
	InpName	head
End

Lifespan	30

###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Waist
		
		Part	:HoldRings.part
		Part	:HoldRings2.part
		Part	:GlowHoldRings.part
		Part	:GlowHoldRings2.part
		Sound fetteringhit 80 80 .75

	End

	Event
		Type	local
		At	Root
		BHVR	Behaviors/RootToKneesAdjustment.bhvr
		Part	:HoldRings.part
		Part	:HoldRings2.part
		Part	:GlowHoldRings.part
		Part	:GlowHoldRings2.part
		Part	:WhiteFire_flame.part

	End
End

End			