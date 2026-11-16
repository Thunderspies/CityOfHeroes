#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Hips
End

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	5

	Event
		EName	hookup
		Type	local
		At	T_Chest
	End

	Event
		EName	Prime
		Type	startPositOnly
		At	hookup
		Geom	Tintable_GEO_WepR_Shuriken
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			TintGeom 1
		End
		Part1	CustomizablePowers_Pools\Epic_WeaponMastery\ShurikenStreaks01.part
		Magnet	Chest
		LookAt	Chest
		Pother	Chest
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Ename 	Boom
		Type 	local
		At 	Chest
		ChildFX :Custom_TrickShot_Pet_Child.fx
	End
End

End
##################################

