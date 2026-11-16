#########################################################
## Vanguard "Redding" Rifle - Sigil Round (Hit)
#########################################################

FxInfo
LifeSpan  45

## INITIAL HIT #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	local 
		At	Chest
		Part1	V_COV\EnemyPowers\Vanguard\Staff\DefensiveAura\ContinuingFX\Hit_Star01.part
		Part4	V_COV\EnemyPowers\Vanguard\Staff\DefensiveAura\ContinuingFX\Hit_Glow01.part
		Part5	V_COV\EnemyPowers\Vanguard\Staff\DefensiveAura\ContinuingFX\Hit_Rays01.part
		Part	V_COV\EnemyPowers\Vanguard\Staff\DefensiveAura\ContinuingFX\Hit_Rays02.part
		LifeSpan	10
	End

End

## SIGIL #######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	GroundSplat
		Type	Start 
		At	Root
		BHVR	:SigilRoundHit\SigilRound_Splat.bhvr
		BhvrOverride
			PyrRotate	0.0 180 0
		End
		Splat	SigilRound_Splat
	End

	Event
		EName 	SigilPulse
		Type	Start 
		At	Root
		part	:SigilRoundHit\SigilPulse01.part
		Lifespan	5
	End

	Event	
		ENAME	Rays
		Type	Start
		At	Root
		part	:SigilRoundHit\SigilGlow01.part
		Lifespan	30
		LifespanJitter	10
	End

	Event	
		ENAME	Rays
		Type	Start
		At	Root
		part	:SigilRoundHit\SigilGlow02.part
		Lifespan	30
		LifespanJitter	10
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
		ENAME	Offset1
		Type	Local
		At	Rotator1
		BhvrOverride
			PositionOffset	0.0 0.0 3.125
		End
		part	:SigilRoundHit\SigilGlow03.part
		Lifespan	30
		LifespanJitter	10
	End

	Event	
		ENAME	Rays
		Type	Local
		At	Rotator1
		BhvrOverride
			PositionOffset	0.0 0.0 -3.125
		End
		part	:SigilRoundHit\SigilGlow03.part
		Lifespan	30
		LifespanJitter	10
	End
End

Condition
	On	Time
	Time	0

	Event	
		ENAME	TrailsRotator
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin		0.0 0.25 0.0
			InitialVelocity	0.0 0.25 0.0
		End
	End

	Event	
		ENAME	TrailsOffset1
		Type	Local
		At	TrailsRotator
		BhvrOverride
			PositionOffset	0.0 0.0 3.125
		End
		part	:SigilRoundHit\Trails01.part
		part	:SigilRoundHit\Trails02.part
		Lifespan	20
		LifespanJitter	10
	End

	Event	
		ENAME	TrailsOffset2
		Type	Local
		At	TrailsRotator
		BhvrOverride
			PositionOffset	0.0 0.0 -3.125
		End
		part	:SigilRoundHit\Trails01.part
		part	:SigilRoundHit\Trails02.part
		Lifespan	20
		LifespanJitter	10
	End
End

#########################################################

End