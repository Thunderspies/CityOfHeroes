#########################################################
## Mitochondria Attack
#########################################################

FxInfo

LifeSpan	90

#########################################################

Condition
	On	Time
	Time	16
	Event
		Type 	Local
		At 	Origin
		Sound mitohit5 60 60 .75
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
		Part1	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\FireHandz.part
		LifeSpan	15
	End
End

#########################################################################
## AREA EFFECT EXPLOSION
#########################################################################

Condition

	On 	Time
	Time 	0

	Event
		EName 	Prime45
		Type	Local 
		At	Hips
		#Part	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Flash1.part
		#Part	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Flash2.part
		Lifespan	7
	End

	Event
		EName 	Prime1
		Type	Local 
		At	Hips
		bhvr    ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Bubble.bhvr

		#Part1	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_BurstCloud1.part
		Part2	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_BurstCloud2.part
		Part3	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Motes1.part
		#Part4	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_Motes2.part
	End

	Event
		EName 	Prime1
		Type	Local 
		At	Hips
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
			Scale			4 4 4
			ScaleRate		1 1 1
			 EndScale		105 105 105
			TrackRate		0.1
			TrackMethod		0
			AnimScale		1.0
			Alpha			16
			LifeSpan		10
		End
		Geom	FX_CytoplasmicAOE
	End

	Event
		EName 	Prime1
		Type	Local 
		At	Hips
		Part1   ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_RingSmall.part
		#Part2   ENEMYFX\DevouringEarth\Hamidon\Power_Mito_CytoplasmicBlast\HitFX\AOE_RingBig.part
	End
End

#########################################################################
## PHYSICS FORCES
#########################################################################

Condition

	On 	Time
	Time 	0

	Event
		Ename	ExplosiveForce
		Type	Local 
		At	Hips
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		60
			PhysForcePower		100
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End
End

#########################################################################

End			