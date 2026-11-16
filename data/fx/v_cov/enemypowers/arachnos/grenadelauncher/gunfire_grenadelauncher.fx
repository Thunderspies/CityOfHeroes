#############################################################
## Gunfire_GrenadeLauncher.fx
#############################################################

FxInfo

LifeSpan 200

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
		sound	shotgun5 120.0 30.0 .9
		LifeSpan 12
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

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	Blast
		Altpiv	1
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	WeaponFx\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End