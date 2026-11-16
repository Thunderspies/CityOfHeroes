#############################################################
## MeleeBlackVomit.fx
#############################################################

FxInfo
LifeSpan	60

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		Type	Local
		At	head
		Sound blackvomit 78 78 .78
		LifeSpan	15
	End
End

Condition
	On 	Time
	Time 	34

	Event
		EName	Spike
		Type	Local
		At	head
		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr
		LifeSpan	15
	End

	Event
		EName	GasStream
		Type	Local
		At	spike
		Altpiv   1
		Part1	:BlackVomitSpit.part
		Part2	:BlackVomitSpitBubbles.part
		Part3	:BlackVomitSpecksSpit.part
		Part3	:BlackVomitSpecks.part
		LifeSpan	15
	End

	Event
		EName	GasSpecks
		Type	Local
		At	spike
		Altpiv   1
		Part1	:BlackVomitBallz.part
		Part2	:BlackVomitSpecks.part
		LifeSpan	15
	End
End

Condition
	On 	Time
	Time 	54

	Event
		Type	start
		At	T_Root
		BHVR	V_COV\Powers\Necromacy\GreenRingSplat.bhvr
		BhvrOverride
			PrimaryTint	70
			SecondaryTint	30
		End
		Splat	Coffee_Ring.tga
		LifeSpan	15
	End


	Event
		Type	start
		At	T_Root
		Part	:BaseSplatter.part
		Part	:BaseBubblesSplatterBLK.part
		Part	:BaseBubblesSplatter.part
		Part	:PoisonSpecksFlat.part
		LifeSpan	15
	End

	Event
		Type	start
		At	T_Chest

		Part	:BaseBubbles.part
		Part	:BaseBubblesDRK.part
		Part	:BaseBubblesDRK2.part
		LifeSpan	28
	End
End

#############################################################

End