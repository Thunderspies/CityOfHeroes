#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	4

	Event
		Type	start
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	Powers\PowerPunch\PowerPunchHit.part
		Part2	Powers\PowerPunch\PowerPunchHit1.part
		Part3	Powers\PowerPunch\PowerPunchHit2.part
		Part4	Powers\PowerPunch\PowerPunchHitRaysSmall.part
		Part1	Powers\PowerPunch\HitRing.part
		Part2	Powers\PowerPunch\PowerPunchBallz.part
		Part3	Powers\PowerPunch\HitRing2.part
		Part4	Powers\PowerPunch\PowerPunchRingz.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:FollowUpGlow.part
		Part2	:FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Lifespan 13
	End
End

Condition
	On	Time
	Time	10

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	:FollowUpRing01.part
		Part3	:FollowUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	:FollowUpRing02.part
		Part4	:FollowUpRing02a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	:FollowUpRingSlow.part
		Part3	:FollowUpRing03.part
		Part4	:FollowUpRing03a.part
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
	End
End

#############################################################

End