#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	100


#########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	WepL

		Part	CustomizeablePowers\Poison\NoxiousGassHand.part
		Part	CustomizeablePowers\Poison\DarkNoxiousGassHand.part
		Part	CustomizeablePowers\Poison\NoxiousGassHandBubble.part
		Part	CustomizeablePowers\Poison\NoxiousGassHandStars.part
		Part	CustomizeablePowers\Poison\NoxiousGassHandGlows.part
		Sound PoisonGasSpew 80 80 .7
		LifeSpan	30

	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName	Prime
		Type	start
		At	WepL

		Bhvr	CustomizeablePowers\Poison\Quillprojectile1.bhvr
		Magnet	T_Root
		LookAt	T_Root

		Part	CustomizeablePowers\Poison\QuillTrailBlack.part
		Part	CustomizeablePowers\Poison\QuillTrail.part
		Part	CustomizeablePowers\Poison\QuillTrailLight.part

		Part	CustomizeablePowers\Poison\QuillTrailStreak.part

		Part	CustomizeablePowers\Poison\QuillBallzTrail.part
		Part	CustomizeablePowers\Poison\QuillBallzTrailBlack.part

	End

End

Condition
	On 	PrimeHit


	Event
		Type	local
		At	T_Root

		Part	CustomizeablePowers\Poison\AOETrailBlack.part
		Part	CustomizeablePowers\Poison\AOETrail.part
		Part	CustomizeablePowers\Poison\AOETrailLight.part

		Part	CustomizeablePowers\Poison\AOETrailStreak.part
		Part	CustomizeablePowers\Poison\AOEYellowSmallBallzTrail.part
		Part	CustomizeablePowers\Poison\AOEYellowSmallBubble.part
		Part	CustomizeablePowers\Poison\AOEYellowSmallstars.part
		Part	CustomizeablePowers\Poison\AOEBallzTrail.part
		Part	CustomizeablePowers\Poison\AOEBallzTrailBlack.part

		LifeSpan	100
	End

	Event
		EName 	Prime
		Type	Destroy


	End

End

End