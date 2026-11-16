#########################################################
##	onfire
##
FxInfo  

LifeSpan	1100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		Sound metal_explo2 110 110 .9

	End

	
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

		Lifespan	1

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	chest
		
		
		Part	:PHS_MetalExplosionBig.part

		Part	:PHS_MetalExplosion01.part

		Part	:PHS_MetalExplosion02.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	chest
		
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

		
		Part	:PHS_MetalBits.part

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	chest
		
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

		Part	:PHS_MetalBits2.part

	End

End

Condition
	Time 	0
	Repeat	2
	Random	1

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
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
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
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
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
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
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
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
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
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
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

	Event
		HardwareOnly
		Type	start
		At	Chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	6
	End
End

Condition
	Time 	0
	Repeat	6

	Event
		SoftwareOnly
		Type	start
		At	Chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
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
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	60
	End

End

Condition
	Time 	0
	Repeat	20

	Event
		HardwareOnly
		Type	start
		At	Chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	6
	End

End

###############  High Energy Shrapnel   ################

Condition
	On 	Time
	Time 	0	
	Event
		Type	Start
		At	chest
		BHVR	Behaviors/CameraShake03Subtle.bhvr
	End

	Event
		EName 	Fire
		Type	Posit
		At	chest
		Part1	WEAPONFX/FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke1.part
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke2.part
		Part1   powers/firecontrol/mineflash.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	chest
		Part1	WEAPONFX/GernadeLauncherSparks.part
		#Part2	WEAPONFX/GernadeLauncherSpark.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
		#Sound	grexplo2 120.0 30.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	chest
		Part	WEAPONFX/FireExplosionFlash.part
		#Part2	WEAPONFX/GernadeLauncherSpark.part
		#Sound	grexplo2 120.0 30.0 1.0
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
		At	chest
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End

	
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	chest
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End

End




End			