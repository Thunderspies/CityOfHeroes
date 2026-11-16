#############################################################
## Assault Rifle - Beanbag Attack FX
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	tracer
		Type	Start
		At	C_Chest
		Bhvr	Behaviors\ShotGun.bhvr
		Geom	Fx_GunBurst
		LifeSpan 3
	End

	Event
		EName 	tracer2
		Type	start
		At	C_Chest
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		sound	shotgun5 120.0 30.0 .9
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	12

	Event
		EName 	Blast
		Type	local
		At	C_Head
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
		At	C_Chest
		bhvr	Behaviors\BeanBagProjectile.bhvr
		BhvrOverride
			TrackRate		4.0
		End
		geom	BeanBag
		Part	:BeanBagDust.part
		Magnet	T_Chest
		LookAt  T_Chest
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