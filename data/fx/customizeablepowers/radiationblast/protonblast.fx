#############################################################
## ProtonBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .3
		Lifespan 65
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings2.Part
	End

	Event
		EName 	Prime
		Type	Start
		At	wepR
		Bhvr	CustomizeablePowers\RadiationBlast\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
			TintGeom		1
		End
		Part3	CustomizeablePowers\RadiationBlast\NeutrinoStreak.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectronsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsLocal.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsStreak.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsStreak.part
		Geom	Tintable_FX_Locknar
		Magnet	Target
		LookAt	Target
		Sound Proton 80 80 .9
	End
End

Condition
	On 	Time
	Time 	19

	Event
		Type	Start
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings2.Part
	End

	Event
		EName 	Prime1
		Type	Start
		At	wepR
		Bhvr	CustomizeablePowers\RadiationBlast\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
			TintGeom		1
		End
		Part3	CustomizeablePowers\RadiationBlast\NeutrinoStreak.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectronsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsLocal.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsStreak.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsStreak.part
		Geom	Tintable_FX_Locknar
		Magnet	Target
		LookAt	Target
		Sound Proton 80 80 .9
	End

End

Condition
	On 	Time
	Time 	31

	Event
		Type	Start
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings2.Part
	End

	Event
		EName 	Prime2
		Type	Start
		At	wepR
		Bhvr	CustomizeablePowers\RadiationBlast\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
			TintGeom		1
		End
		Part3	CustomizeablePowers\RadiationBlast\NeutrinoStreak.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectronsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsLocal.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsStreak.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsStreak.part
		Geom	Tintable_FX_Locknar
		Magnet	Target
		LookAt	Target
		Sound Proton 80 80 .9
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName	Handburst
		Type	start
		At	wepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings2.Part
	End

	Event
		Type	Start
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings2.Part
	End

	Event
		EName 	Prime3
		Type	Start
		At	wepR
		Bhvr	CustomizeablePowers\RadiationBlast\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
			TintGeom		1
		End
		Part3	CustomizeablePowers\RadiationBlast\NeutrinoStreak.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectronsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsLocal.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsStreak.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsStreak.part
		Geom	Tintable_FX_Locknar
		Magnet	Target
		LookAt	Target
		Sound Proton 80 80 .9
	End

End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End
End

#############################################################

End