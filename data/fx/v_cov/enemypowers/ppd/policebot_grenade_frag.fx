#########################################################
##	
##

FxInfo
LifeSpan  200


Input  
	InpName	Origin
End

Input  
	InpName	T_Root
End

Input  
	InpName	WepR
End

Input  
	InpName	T_Chest
End

##################################
Condition
	On 	Time
	Time 	31

	Event
		EName 	weporigin
		Type	local 
		At	C_Head
		BhvrOverride
			PositionOffset	0 -.25 -2.2
			PyrRotate	0 90 0
		End
		
	End

	Event
		EName 	Prime
		Type	start 
		At	weporigin
		geom	grenade
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		BhvrOverride
			InitialVelocity		0 2 0
			Drag			.01
			Gravity			.015
		End
		Part1	WEAPONFX/BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

	Event
		EName 	Blast
		Type	Start
		At	weporigin
		Geom	Fx_GunBurst
		Sound Grenade2 120.0 120.0 1.0
		
	End

	Event
		EName 	tracer
		Type	start
		At	weporigin

		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
	End

End

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
		Part1	WEAPONFX/FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	WEAPONFX/GernadeLauncherSparks.part
		#Part2	WEAPONFX/GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	WEAPONFX/FireExplosionFlash.part
		#Part2	WEAPONFX/GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End
	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

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