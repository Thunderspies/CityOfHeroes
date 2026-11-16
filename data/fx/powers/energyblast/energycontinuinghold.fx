#########################################################
##	cot_waterfall_med.fx
##
FxInfo

###########################################################




Condition
	On	Time	
	Time 	18

	Event
		Type 	Local
		At	Origin
		Sound Energyhold_loop 70.0 70.0 .62
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End

End







Condition
	On	Cycle
	Time	3
	Event
		Type	start
		At	FootL
		Splat	Ember
		Bhvr	:Splat.bhvr
		LifeSpan	13

	End
End

Condition

	Event
		Type	local
		At	FootR
		Part	Powers/ColdControl/EnergyIceChunks.part
		Part1	:EnergyBallsHold.part
		Part	:EnergyBallsHoldHitWhite.part
		Part3	:SolidEnergyBallsHoldHit2.part
		Part4	:BlastHoldHitRingsa.part

	End
	
	Event
		Type	local
		At	LlegR
		Part1	:smallEnergyBallsHold.part
		Part	:smallEnergyBallsHoldHitWhite.part
		Part3	:smallSolidEnergyBallsHoldHit2.part
		Part4	:smallBlastHoldHitRingsa.part

	End

End

Condition
	On	Cycle
	Time	3
	Event
		Type	start
		At	FootR
		Splat	Ember
		Bhvr	:Splat.bhvr
		LifeSpan	13

	End
End

Condition

	Event
		Type	local
		At	LlegL
		Part1	:smallEnergyBallsHold.part
		Part	:smallEnergyBallsHoldHitWhite.part
		Part3	:smallSolidEnergyBallsHoldHit2.part
		Part4	:smallBlastHoldHitRingsa.part

	End

	Event
		Type	local
		At	FootL
		Part	Powers/ColdControl/EnergyIceChunks.part
		Part1	:EnergyBallsHold.part
		Part	:EnergyBallsHoldHitWhite.part
		Part3	:SolidEnergyBallsHoldHit2.part
		Part4	:BlastHoldHitRingsa.part

	End
end

End