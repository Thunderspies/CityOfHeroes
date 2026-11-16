#########################################################
## Mitochondria Attack
#########################################################

FxInfo

LifeSpan 105

#########################################################

Condition
	On	Time
	Time	16
	Event
		Type 	Local
		At 	Origin
		Sound mitoblast1 80 80 .88
	End
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	Waist
		Part1	:FireHandz.part
		LifeSpan	15
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start
		At	Waist
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		BhvrOverride
			TrackRate		6
			TrackMethod		2
		End
		Magnet	Target
		LookAt	Target
		Part	:Head.part
		Part2	:FireBoltTail2.part
		Part1	:FireHands2.part
		Part3	:FireBoltTail.part
	End
End

#########################################################################
## AREA EFFECT EXPLOSION
#########################################################################

Condition

	On	PrimeHit

	Event
		EName 	Hookup
		Type	Local
		At	T_Root
		Geom	RootToChestAdjustment
		Sound mitohit5 60 60 .75
	End

	Event
		EName 	Prime45
		Type	Local
		At	T_Hips
		Part	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Flash1.part
		Lifespan	7

	End

	Event
		Type	Local
		At	T_Root
		Part	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Flash2.part
		Lifespan	7
	End

	Event
		EName 	Prime1
		Type	Local
		At	Hookup
		Altpiv	1
		bhvr    ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Bubble.bhvr

		Part1	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_BurstCloud1.part
		Part2	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_BurstCloud2.part
		Part3	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Motes1.part
		Part4	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Motes2.part
	End

	Event
		EName 	Prime8
		Type	Local
		At	Hookup
		Altpiv	1
		Part1   ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_RingSmall.part
		Part2   ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_RingBig.part
	End

	Event
		EName 	AOEGeo
		Type	Local
		At	Hookup
		bhvr	behaviors/ScaleBubble.bhvr
		bhvrOverride
			StartJitter		0.0 0.0 0.0
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Gravity			0.0
			Spin			0.0 0.01 0.0
			SpinJitter		0.0625 0.0625 0.0625
			FadeInLength		0
			FadeOutLength		10
			Scale			8 8 8
			ScaleRate		1 1 1
			 EndScale		90 90 90
			TrackRate		0.1
			TrackMethod		0
			AnimScale		1.0
			Alpha			16
			LifeSpan		10
		End
		Geom	FX_CytoplasmicAOE
	End
End

#########################################################

Condition
	On 	Time
	Time 	24

	Event
		EName	SwordFlash2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName	InitialSwordFire
		Type	Destroy
	End
End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

End

#########################################################

End