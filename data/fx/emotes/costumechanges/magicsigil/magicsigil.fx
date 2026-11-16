#########################################################
## Vanguard "Redding" Rifle - Sigil Round (Hit)
#########################################################

FxInfo

LifeSpan 120

## SIGIL #######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	GroundSplat
		Type	Start
		At	Root
		BHVR	:Sigil_Splat.bhvr
		BhvrOverride
			PyrRotate	0.0 180 0
		End
		Splat	CircleTriangleShape.tga
		Lifespan 60
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	Rotator1
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin	0 0.0625 0
		End
	End

	Event
		Type	Local
		At	Rotator1
		BhvrOverride
			PositionOffset	0.0 0.0 3.125
		End
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\SigilGlow03.part
		Lifespan 60
		LifespanJitter 10
	End

	Event
		Type	Local
		At	Rotator1
		BhvrOverride
			PositionOffset	0.0 0.0 -3.125
		End
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\SigilGlow03.part
		Lifespan 60
		LifespanJitter 10
	End

	Event
		Type	Start
		At	Root
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\SigilGlow01.part
		Lifespan 60
		LifespanJitter 10
	End

	Event
		Type	Start
		At	Root
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\SigilGlow02.part
		Lifespan 60
		LifespanJitter 10
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName 	SigilPulse
		Type	Start
		At	Root
		part	:VanguardSigil_Pulse1.part
	End

	Event
		ENAME	TrailsRotator
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin		0.0 0.2 0.0
			InitialVelocity	0.0 0.2 0.0
			Drag		0.025
		End
	End

	Event
		Type	Local
		At	TrailsRotator
		BhvrOverride
			PositionOffset	0.0 0.0 3.125
		End
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\Trails01.part
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\Trails02.part
		Lifespan 40
		LifespanJitter 10
	End

	Event
		Type	Local
		At	TrailsRotator
		BhvrOverride
			PositionOffset	0.0 0.0 -3.125
		End
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\Trails01.part
		part	V_COV\EnemyPowers\Vanguard\Rifle\SigilRoundHit\Trails02.part
		Lifespan 40
		LifespanJitter 10
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	FootL
		ChildFX	:Child_VanguardBodyGlow.fx
	End
End

#########################################################

End