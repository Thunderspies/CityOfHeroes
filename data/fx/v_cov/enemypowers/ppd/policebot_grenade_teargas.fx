#########################################################
##	
##

FxInfo
LifeSpan  240


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
		EName 	Smoke
		Type	Posit
		At	T_Root
		Part1	V_COV/WEAPONFX/TearGasExplodeSmoke1.part
		Part1	V_COV/WEAPONFX/TearGasExplodeSmoke2.part
		
		Part	POWERS\ItemsAndTempPowers\TearGas01.part
		Part1	POWERS\ItemsAndTempPowers\TearGasFlat.part
		Lifespan 60
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	WEAPONFX/FireExplosionFlash.part
		Part2	V_COV/WEAPONFX/DynamiteExplodeSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
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