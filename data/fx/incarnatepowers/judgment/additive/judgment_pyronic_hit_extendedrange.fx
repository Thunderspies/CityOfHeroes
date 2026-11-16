#############################################################
## XXLargeFireBall.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound 	Incarnate_PyronicJudgement_Hit_01 100 100 .8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Origin
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		Bhvr	behaviors/CameraShake02.bhvr
		At	Origin
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:Judgment_Pyronic_Flash.part
		Part	:Judgment_Pyronic_Flash2.part
		Part	:Judgment_Pyronic_AreaFire_Mult.part
		Part	:Judgment_Pyronic_AreaFire_Range.part
		Part	:Judgment_Pyronic_AreaAmber.part
		Part	:Judgment_Pyronic_AreaAshes.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	2


	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	Origin
		Part	:Judgment_Pyronic_AreaFireB_Mult.part
		Part	:Judgment_Pyronic_AreaFireB_Range.part
		Lifespan 3
	End

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	Origin
		BhvrOverride
          		PositionOffset 0 0.25 0

         	End
		Part	:Judgment_Pyronic_FloorFlame_Range.part
		Part	:Judgment_Pyronic_AreaBurnedEarth_Range.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	6
	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	Origin
		Part	:Judgment_Pyronic_AreaFireB_Mult.part
		Part	:Judgment_Pyronic_AreaFireB.part
		Lifespan 3
	End

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	Origin
		Part	:Judgment_Pyronic_Smoke_Range.part
		Part	:Judgment_Pyronic_SmokeB.part
		Part	:Judgment_Pyronic_Smoke_Lingering.part
		Lifespan 10
	End
End

#############################################################

End