#############################################################
## Gunfire_HolyShotgun.fx
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
		Geom	Fx_GunBurstHolyShotgun
		Bhvr	Behaviors\ShotGun.bhvr
		LifeSpan 2
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	1
		Part	:Gunfire_HolyBuckShot.part
		Part	:Gunfire_HolyShotgunMuzzflash.part
		Part	:Gunfire_HolyShotgunsmoke.part
		Part	:Gunfire_HolyShotgunMuzzflashSmall.part
		sound	shotgun5 120.0 30.0 .9
		LifeSpan 12
	End
End

#############################################################

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
		bhvr	Behaviors\BeanBagProjectile.bhvr
		BhvrOverride
			TrackRate		4.0
		End
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