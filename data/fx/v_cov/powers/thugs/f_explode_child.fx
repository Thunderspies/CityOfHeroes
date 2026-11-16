#########################################################
##	
##

FxInfo
LifeSpan  200

##################################

Condition
	On 	PrimeHit
	
	Event
		Type	Start
		At	T_Root
		BHVR	Behaviors/CameraShake01Subtle.bhvr
	End

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	WeaponFX\FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	WeaponFX\GernadeLauncherSparks.part
		#Part2	WeaponFX\GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	WeaponFX\FireExplosionFlash.part
		#Part2	WeaponFX\GernadeLauncherSpark.part
		
		Lifespan 5
		
	End
End

Condition
	On	Primehit
		
	Repeat	24
	
	Event
		EName	Sparkout
		Type 	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		
	End

End


##################################

Condition
	On 	Time
	Time	34

	Event
		Ename 	Blast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	43

	Event
		Ename 	tracer
		Type	Destroy
	End

End

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End


End


End							