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
		EName	Hand1
		Type	Local
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .3
		LifeSpan	65
	End
End


Condition
	On 	Time
	Time 	10	#25

	Event
		EName	Handburst
		Type	Local
		At	WepR
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part

	End

	Event
		EName 	Prime
		Type	Start
		At	wepR
		Bhvr	behaviors\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
		End
		Geom	FX_Locknar
		Magnet	Target
		LookAt	Target
		Part3	:NeutrinoStreak.part
		Sound Proton 80 80 .9

	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	Prime
		Part1	:RadiationInfectOthersHandsLocal.part
		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part

	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	Prime
		#Part1	:RadiationInfectOthersHands.part
		#Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part

	End
End

Condition
	On 	Time
	Time 	19	#34

	Event
		EName	Handburst
		Type	Local
		At	WepR
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part

	End

	Event
		EName 	Prime1
		Type	Start
		At	wepR
		Bhvr	behaviors\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
		End
		Geom	FX_Locknar
		Geom	FX_Locknar
		Magnet	Target
		LookAt	Target
		Part3	:NeutrinoStreak.part
		Sound Proton 80 80 .8

	End

	Event
		EName	InitialSwordFire2
		Type	Local
		At	Prime1
		Part1	:RadiationInfectOthersHandsLocal.part
		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part

	End

	Event
		EName	InitialSwordFire2
		Type	Local
		At	Prime1
		#Part1	:RadiationInfectOthersHands.part
		#Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part

	End
End

Condition
	On 	Time
	Time 	31	#47

	Event
		EName	Handburst
		Type	Local
		At	wepR
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part

	End

	Event
		EName 	Prime2
		Type	Start
		At	wepR
		Bhvr	behaviors\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
		End
		Geom	FX_Locknar
		Geom	FX_Locknar
		Magnet	Target
		LookAt	Target
		Part3	:NeutrinoStreak.part
		Sound Proton 80 80 .8

	End

	Event
		EName	InitialSwordFire2
		Type	Local
		At	Prime2
		Part1	:RadiationInfectOthersHandsLocal.part
		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part

	End

	Event
		EName	InitialSwordFire2
		Type	Local
		At	Prime2
		#Part1	:RadiationInfectOthersHands.part
		#Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part

	End
End

Condition
	On 	Time
	Time 	42	#67
	Event
		EName	Handburst
		Type	start
		At	wepR
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part

	End

	Event
		EName 	Prime3
		Type	Start
		At	wepR
		Bhvr	behaviors\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
		End
		Geom	FX_Locknar
		Geom	FX_Locknar
		Magnet	Target
		LookAt	Target
		Part3	:NeutrinoStreak.part
		Sound Proton 80 80 .9

	End

	Event
		EName	InitialSwordFire2
		Type	Local
		At	Prime3
		Part1	:RadiationInfectOthersHandsLocal.part
		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part


	End

	Event
		EName	InitialSwordFire2
		Type	Local
		At	Prime3
		#Part1	:RadiationInfectOthersHands.part
		#Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part

	End
End

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

End


