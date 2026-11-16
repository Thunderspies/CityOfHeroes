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
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
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
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part
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
		Part3	:NeutrinoStreak.part
		Part3	:NeutrinoStreak_Alpha.part

		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part
		Geom	Tintable_Dark_FX_Locknar
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
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part
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
		Part3	:NeutrinoStreak.part
		Part3	:NeutrinoStreak_Alpha.part

		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part
		Geom	Tintable_Dark_FX_Locknar
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
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part
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
		Part3	:NeutrinoStreak.part
		Part3	:NeutrinoStreak_Alpha.part

		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part
		Geom	Tintable_Dark_FX_Locknar
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
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part
	End

	Event
		Type	Start
		At	WepR
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part
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
		Part3	:NeutrinoStreak.part
		Part3	:NeutrinoStreak_Alpha.part

		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part
		Geom	Tintable_Dark_FX_Locknar
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