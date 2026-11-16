#############################################################
## ENCOUNTER_CustomRifle_Slug.fx
#############################################################

FxInfo

LifeSpan  60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	C_Chest
		Bhvr	Behaviors\ShotGun.bhvr
		Geom	Fx_GunBurst
		LifeSpan 2
	End

	Event
		EName 	tracer2
		Type	start
		Type	Start
		At	C_Chest
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		sound	MachGun_single 120.0 30.0 .9
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Blast
		Type	local
		Type	Start
		At	C_Head
		ChildFX	V_COV\PhysicsEnabled\ShotGunShellEject.fx
	End
End

#############################################################

End