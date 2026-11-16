#########################################################
##	
##

FxInfo
LifeSpan  150

Input  
	InpName	Target
End

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	40


	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	Mastermind_Military_Rifle
		
	End

	Event
		EName 	tracer
		Type	Start
		At	Blast
		Altpiv	1
		BHVR	WEAPONs\ShotgunWeaponRotate.bhvr
		Geom	Fx_GunBurst
		LifeSpan  3
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	1
		

		BHVR	WEAPONs\WeaponRotate2.bhvr
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		sound	mercenary_singleShot 120.0 30.0 1.0
		
	End

End

Condition
	On 	Time
	Time 	55


	Event
		EName 	Blast
		Type	local
		At	wepR
		#Part1	WEAPONFX/Gunfire_ShotgunShell.Part
		ChildFX	V_COV\PhysicsEnabled\ShotGunShellEject.fx
	End

End

##################################

Condition
	On 	Time
	Time	42

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	50

	Event
		Ename 	tracer2
		Type	Destroy
	End

End


End				