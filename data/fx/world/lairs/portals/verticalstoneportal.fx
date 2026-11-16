#########################################################
##	RadiationAura
FxInfo


##################################
Condition
	On	Time
	Time	0

	
##########################################################

	Event
		EName	Ring2
		Type	local
		At	Root
		Geom	ElectricalMeleeBubble
		BHVR	:PortalPivotCore.bhvr
		Part	:RockGlowsUp.part
		
	End


	Event
		EName	Ring2
		Type	local
		At	Root
		BHVR	:PortalPivotNoScale.bhvr
		Part	:RockLineGlowsUp.part
	End

	Event
		EName	Ring2
		Type	local
		At	Root
		BHVR	:PortalPivot.bhvr	
		Part	:RockLineUp.part
	End
	
	Event
		EName	Ring2
		Type	local
		At	Root
		BHVR	:PortalPivotUp.bhvr
		Part	:RockLineDown.part
		
	End

	
	Event
		EName	Ring2
		Type	local
		At	Root
		BHVR	:aPortalPivot.bhvr
		Part	:Ripples.part	
		Part	:RippleRings.part
		Part	:RippleThinRings.part
	End

End


End				