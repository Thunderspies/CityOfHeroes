#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

LifeSpan  60

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Start
		At	C_Chest
		Bhvr	Behaviors/ShotGun.bhvr
		Geom	Fx_GunBurst
		sound	mercenary_singleShot 120.0 30.0 1.0
		LifeSpan 2
	End

	Event
		Type	Start
		At	C_Chest
		ChildFX	:Player_AssaultRifle_SingleShot_Child.fx
		Lifespan 3
	End
End

#############################################################

End