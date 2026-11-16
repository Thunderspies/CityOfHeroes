#########################################################
##	
##

FxInfo
LifeSpan  210

Input  
	InpName	origin
End

Input  
	InpName	T_Chest
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End
##################################
Condition
	On 	Time
	Time 	80

	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	Nemesis_Blaster
		LifeSpan  110
	End

	Event
		EName 	tracer
		Type	Start
		At	Blast
		Altpiv	1
		Geom	Fx_GunBurst
		Bhvr	Behaviors/ShotGun.bhvr
		LifeSpan  110
	End

	Event
		EName 	fire
		Type	start
		At	Blast
		Altpiv	1
		
		Part	:TorchflameA.part
		Part	:TorchflameB.part
		Part	:SniperFlash.part
		Part	:SniperGlow.part

		LifeSpan	1

	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	2
		
		sound	shotgun3 120.0 30.0 .9
		LifeSpan	9

		Part	:nemesisMuzzflashCore.part
		Part	:nemesisMuzzflash.part
		Part	:nemesisMuzzflashRing.part
		Part	:nemesisMuzzflashRing2.part
		
		
	End
	
	Event
		EName 	smoke
		Type	start
		At	Blast
		Altpiv	2
		
		LifeSpan	11
		
		Part	:nemesisMuzzflashSmoke.part
			
	End

	Event
		EName 	Prime
		Type	Start
		At	Blast
		Altpiv	1
		Geom	FX_CannonBall
		Bhvr	Behaviors\CannonBallProjectile.bhvr
		Part	:CannonBallSmoke.part
		Magnet T_Chest
		Lookat T_Chest

	End
End


##################################



Condition
	On 	Time
	Time	86

	Event
		Ename 	tracer
		Type	Destroy
	
	End

End				

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Chest
		Part1	:FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part1	:GernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part	:FireExplosionFlash.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End
	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		300
			PhysForcePowerJitter	25
		End

		Lifespan	1

	End

	Event
		Ename 	Prime
		Type	Destroy
	
	End

End

End				