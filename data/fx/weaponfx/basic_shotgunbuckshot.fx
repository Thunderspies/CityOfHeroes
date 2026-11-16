#########################################################
##
##

FxInfo
LifeSpan  40

Input
	InpName	Target
End

Input
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	1


	Event
		EName 	Blast
		Type	local
		At	WepR
		Geom	Sniper01

	End

	Event
		EName 	tracer
		Type	Start
		At	Blast
		Altpiv 1
		Geom	Fx_GunBurst
		Bhvr	Behaviors/ShotGun.bhvr
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	1

		Part	:Gunfire_BuckShot.part
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		sound	shotgun2 120.0 30.0 .9
		sound	shotgun5 120.0 30.0 .9
		sound	shotgun7 120.0 30.0 .9

	End

End



##################################

Condition
	On 	Time
	Time	3

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	10

	Event
		Ename 	tracer2
		Type	Destroy
	End

End


End