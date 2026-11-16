#########################################################
##	RadiationAura
FxInfo


##################################

Condition
	On	Time
	Time	0

	Event
		EName	AntiMatter
		Type	local
		At	Root
		BHVR	Behaviors/PortalPivot.bhvr
		Part	:PortalRings.part
		Part	:PortalSpecks.part
		Part	:ElectricitySparks.part	
		Part	:ElectricitySparks2.part
		Part	:RiktiPortalRing.part
		Part	:PortalThickness.part
		Sound eleccage_loop 60 60 .17
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	Ring2
		Type	local
		At	Root
		BHVR	Behaviors/PortalPivot.bhvr
		Part	:PortalRingPulse.part
		Part	:PortalRingPulse2.part
		Part	:PortalOuterRing.part
	End

End


End				