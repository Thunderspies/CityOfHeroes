#########################################################
##	Fire_Ball
##

#########################################################

FxInfo
LifeSpan	200




#########################################################

Condition
	On 	Time
	Time 	25

	Event
		EName	Prime
		Type	start
		At	LarmL

		Bhvr	CustomizeablePowers\Poison\GreenGlassScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_PoisonGlassBall
		Magnet	Target
		LookAt	Target

		Part	CustomizeablePowers\Poison\LiquidTrailBlackQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlackQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlackQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrail.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlack.part
		Part	CustomizeablePowers\Poison\LiquidTrail.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlack.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlack.part

		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuickSmallThin.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuickSmallThin.part

	#		Part	:QuillTrailStreak.part

	#		Part	:QuillTrail.part
	#		Part	:QuillTrailLight.part

	#		Part2	weaponFx/QuillTrailSpecks.part
	#		Part3	weaponFx/QuillBubblesTrail.part
		Part5	CustomizeablePowers\Poison\SmallBallzTrail.part
		Part5	CustomizeablePowers\Poison\SmallBallzTrailBlack.part
		Part5	CustomizeablePowers\Poison\SmallFallingDrips.part
		Part5	CustomizeablePowers\Poison\SmallFallingDripsBlack.part
		Sound Palmpoisonblast 80 80 .8

	End

	Event
		EName	Prime2
		Type	local
		At	Prime

		Part	CustomizeablePowers\Poison\PoisonHeadLocalBlack.part
		Part	CustomizeablePowers\Poison\PoisonHeadLocalBlack.part
		Part	CustomizeablePowers\Poison\PoisonHeadLocalBlack.part
		Part	CustomizeablePowers\Poison\PoisonHeadLocal.part
		Part	CustomizeablePowers\Poison\PoisonHeadLocal.part
		Part	CustomizeablePowers\Poison\PoisonHeadLocal.part
		Part	CustomizeablePowers\Poison\PoisonHeadLocal.part

	End


End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy


	End

	Event
		EName 	Prime2
		Type	Destroy


	End
End

End