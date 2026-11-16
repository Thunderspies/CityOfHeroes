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
	Time 	75


	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	LaserAssault
		
	End

	Event
		EName 	tracer
		Type	Start
		At	Blast
		Altpiv	1
		Geom	Fx_GunBurst
		Bhvr	Behaviors/ShotGun.bhvr
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	1
		

		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		sound	lee1 100.0 70.0 .8
		
	End

End

Condition
	On 	Time
	Time 	81


	Event
		EName 	Blast
		Type	local
		At	Blast
		Altpiv	2
		#Part1	WEAPONFX/Gunfire_ShotgunShell.Part
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End

End

##################################

Condition
	On 	Time
	Time	77

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	105

	Event
		Ename 	tracer2
		Type	Destroy
	End

End


End				