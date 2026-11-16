#########################################################
##	FireRing
##
FxInfo

Condition

	Event
		Type	local
		At	Waist
		Sound radiation5_loop 70.0 70.0 .4
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End



Condition

	Event
		Type	local
		At	Waist
		
		Part1	:BlastHoldGlow.part
		Part2	:RadiationHandElectronsLocal2.part
		Part3	:ElectronsRings.part
		Part5	:ElectronsLocal.part
		Part	:HoldRings.part
		Part	:HoldRings2.part
	End

End


End			