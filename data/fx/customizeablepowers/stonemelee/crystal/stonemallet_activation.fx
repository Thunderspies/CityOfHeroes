#############################################################
## StoneMallet_Activation.fx
#############################################################

FxInfo

Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	RockBase
		Type	start
		At	WepR
		Sound stoneweaponpickup4 80 80 0.85
		lifeSpan 80
	End

	Event
		EName 	mallet
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\StoneMelee\EarthWeapon.bhvr
		BhvrOverride
			PyrRotate		0 0 180
			TintGeom		1
		End
		Geom	Tintable_CrystalMallet
	End

	Event
		EName 	Dust
		Type	local
		At	WepR
		Part	CustomizeablePowers\StoneMelee\EarthWeaponDust.part
		Part	CustomizeablePowers\StoneMelee\EarthWeaponDust2.part
		Part	CustomizeablePowers\StoneMelee\Lava\WeaponRocks04a.part
		Part	CustomizeablePowers\StoneMelee\Lava\WeaponRocks03a.part
	End
End

#############################################################

End