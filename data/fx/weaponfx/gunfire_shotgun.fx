#############################################################
## Gunfire_Shotgun.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	LaserAssault
	End

	Event
		Type	Start
		At	Blast
		Altpiv	1
		Bhvr	Behaviors\ShotGun.bhvr
		Geom	Fx_GunBurst
		LifeSpan 2
	End

	Event
		Type	start
		At	Blast
		Altpiv	1
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		sound	MachGun_single 120.0 30.0 .9
		LifeSpan 1
	End
End

Condition
	On 	Time
	Time 	12

	Event
		EName 	Blast
		Type	local
		At	Blast
		Altpiv	2
		ChildFX	V_COV\PhysicsEnabled\ShotGunShellEject.fx
	End
End

#############################################################

End