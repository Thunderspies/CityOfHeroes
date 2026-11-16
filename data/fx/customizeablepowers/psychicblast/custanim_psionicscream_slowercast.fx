#############################################################
## PsionicScream_SlowerCast.fx
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		EName	core
		Type	Local
		At	Mystic
		Geom	LocalPivot01
		Sound mental11 80 80 0.8
	End

	Event
		EName	EmitterParent
		Type	Local
		At	Core
		AltPiv	4
	End

	Event
		EName	EmitterOffset
		Type	PositOnly
		At	EmitterParent
		LookAt	Target
	End

	Event
		EName	Emitter
		Type	Local
		At	EmitterOffset
		BhvrOverride
			PYRRotate	-90 0 0
			PositionOffset	  0 .3 0
		End
	End

	Event
		EName	core3
		Type	Local
		At	Emitter
		part1	:CryRays.part
		part2	:PsionicGlow2.part
		Lifespan 32
	End

	Event
		EName	core2
		Type	Local
		At	Emitter
		part1	:PsionicRingsSmall.part
		part2	:PsionicRingzSmall.part
		Lifespan 31
	End
End

##########################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	OrbStreak
		Type	Local
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors\Psionic_Spin.bhvr
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Orb
		Type	Local
		At	OrbStreak
		altpiv  1
		Part1	:GlowOrbStar2Warm.part
		part3	:GlowOrbTailWarm2.part
		Lifespan 33
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	corez
		Type	Local
		At	head
		Geom	PsionicRigging

	End
End

###########################################################################

Condition
	On	Time
	Time	30

	Event
		EName	prime
		Type	Start
		At	Corez
		Altpiv	2
		bhvr	behaviors\PsionicWave.bhvr
		BhvrOverride
			TrackRate		1.7
		End
		Geom	FrontFacingLocalPivot
		Magnet	target
		LookAt  target
	End

	Event
		Type	Local
		At	Prime
		AltPiv	1
		bhvr	behaviors\PushProjectile2.bhvr
		BhvrOverride
			TintGeom		1
			Alpha			64
		End
		Geom	Tintable_PsiTeleWave
		Lifespan 5
	End

	Event
		ENAME	Ringsz
		Type	Local
		At	Prime
		AltPiv	1
		bhvr	behaviors\PushProjectile3.bhvr
		Part1	:BlastStreakWidened.part
		Lifespan 11
	End
End


###############################################################################

Condition
	On	Time
	Time	40

	Event
		EName	coreza
		Type	Local
		At	head
		Geom	PsionicRigging
	End

	Event
		EName	prime1
		Type	Start
		At	Corez
		Altpiv	2
		bhvr	behaviors\PsionicWave.bhvr
		Geom	FrontFacingLocalPivot
		Magnet	target
		LookAt  target
	End

	Event
		ENAME	Geoma
		Type	Local
		At	Prime1
		AltPiv	1
		bhvr	behaviors\PushProjectile2.bhvr
		BhvrOverride
			TintGeom		1
			Alpha			64
		End
		Geom	Tintable_PsiTeleWave
		Lifespan 5
	End

	Event
		ENAME	Ringsza
		Type	Local
		At	Prime1
		AltPiv	1
		bhvr	behaviors\PushProjectile3.bhvr
		Part1	:BlastStreakWidened.part
		Lifespan 11
	End
End

#############################################################

End