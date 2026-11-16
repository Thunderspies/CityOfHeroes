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
	Time 	30


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
		LifeSpan  3
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	1
		

		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		sound	shotgun3 120.0 30.0 .9
		
	End

End

Condition
	On 	Time
	Time 	45


	Event
		EName 	Blast
		Type	local
		At	Blast
		Altpiv	2
		Part1	WEAPONFX/Gunfire_ShotgunShell.Part
	End

End

##################################

Condition
	On 	Time
	Time	32

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	40

	Event
		Ename 	tracer2
		Type	Destroy
	End

End


End				