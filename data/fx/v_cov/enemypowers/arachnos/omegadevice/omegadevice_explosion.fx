#########################################################
## Lord Recluse - Arachnos PTT Buff
#########################################################

FxInfo

Lifespan 150

### EXPLOSION ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Hips
		Bhvr	Scripted\LordReclusePTTBuff\CameraShake.bhvr
		lifespan 31
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		750
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	chest
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion02.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	Hips
		Part1	WEAPONFX\FireExplosion.part
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke1.part
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke2.part
		Part1   powers\firecontrol\mineflash.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Hips
		Part1	WEAPONFX\GernadeLauncherSparks.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
		Lifespan 5
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Hips
		Part	WEAPONFX\FireExplosionFlash.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	0
	Repeat	24

	Event
		EName	Sparkout
		Type 	Start
		At	Hips
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat	24

	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Hips
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

## RED ENERGY EXPLOSION ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Hips
		Part1	Scripted\LordReclusePTTBuff\Explosion_ThinFastRings.part
		Part2	Scripted\LordReclusePTTBuff\Explosion_BigLightRays1.part
		lifespan 35
	End

	Event
		Type	start
		At	Hips
		Part2	Scripted\LordReclusePTTBuff\Explosion_BigLightRays2.part
		lifespan 20
	End

	Event
		Type	start
		At	Hips
		Part1	Scripted\LordReclusePTTBuff\Explosion_Cloud.part
		Part2	Scripted\LordReclusePTTBuff\Explosion_CloudDecal.part
		Part3	Scripted\LordReclusePTTBuff\Explosion_Cloud_Additive.part
		Part4	Scripted\LordReclusePTTBuff\Explosion_CloudDecal_Additive.part
		lifespan 5
	End
End

## PHYSICS DEBRIS ###########################################################

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start
		At	Hips
		BhvrOverride
			InitialVelocity		0.0 0.28 0.0
			InitialVelocityJitter	0.1 0.2 0.1
			Drag			0.006
			physics 		1
			physRadius 		0.12
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			Gravity 		.04
		End
		Part	Explosions\MetalExplosion\PHS_MetalBits.part
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start
		At	Hips
		BhvrOverride
			InitialVelocity		0.0 0.28 0.0
			InitialVelocityJitter	0.1 0.2 0.1
			Drag			0.006
			physics 		1
			physRadius 		0.12
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			Gravity 		.04
		End
		Part	Explosions\MetalExplosion\PHS_MetalBits2.part
	End
End

Condition
	Time 	0
	Repeat	2
	Random	1

	Event
		Type	start
		At	Hips
		Bhvr	Behaviors\BlackMetalOffset.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	Hips
		Bhvr	Behaviors\BlackMetalOffset2.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	Hips
		Bhvr	Behaviors\BlackMetalOffset3.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	Hips
		Bhvr	Behaviors\BlackMetalOffset4.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	Hips
		Bhvr	Behaviors\BlackMetalOffset5.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End
End

Condition
	Time 	0
	Repeat	6

	Event
		SoftwareOnly
		Type	start
		At	Hips
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	60
	End
End

Condition
	Time 	0
	Repeat	2
	Random	1

	Event
		HardwareOnly
		Type	start
		At	Hips
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2.5
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	6
	End
End

Condition
	Time 	0
	Repeat	20

	Event
		HardwareOnly
		Type	start
		At	Hips
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2.5
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	6
	End
End

#############################################################

End