#############################################################
## XXLargeFireBall.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################


## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		Part1   :judgement_pyronic_projectilemiddle.part
		Part2   :judgement_pyronic_projectile.part
		Magnet	Target
		LookAt	Target
		Lifespan 30
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Type	Start
		At	target
		ChildFX	:Judgment_Pyronic_Hit.fx
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	target
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
		At	target
		Part	:Judgment_Pyronic_AreaFire.part
		Part	:Judgment_Pyronic_AreaFireB.part
		Lifespan 3
	End

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	target
		BhvrOverride
          		PositionOffset 0 -4 0

         	End
		Part	:Judgment_Pyronic_FloorFlame.part
		Part	:Judgment_Pyronic_AreaBurnedEarth.part
		Lifespan 1

	End

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	target
		Part	:Judgment_Pyronic_smoke.part
		Lifespan 10
	End
End

#############################################################

End