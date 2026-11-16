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
		EName	Spike
		Type	Local
		At	head
		bhvr	CustomizeablePowers\Poison\scale1a.bhvr
		Sound poisonspit2 100 100 .8
	End

	Event
		EName	Prime
		Type	start
		At	Spike
		Altpiv	1

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
		Part	CustomizeablePowers\Poison\LiquidTrail.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlack.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlack.part
		Part	CustomizeablePowers\Poison\LiquidTrailBlack.part

		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuick.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuickSmallThin.part
		Part	CustomizeablePowers\Poison\LiquidTrailQuickSmallThin.part

		Part	CustomizeablePowers\Poison\SmallBallzTrail.part
		Part	CustomizeablePowers\Poison\SmallBallzTrailBlack.part
		Part	CustomizeablePowers\Poison\SmallFallingDrips.part
		Part	CustomizeablePowers\Poison\SmallFallingDripsBlack.part

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