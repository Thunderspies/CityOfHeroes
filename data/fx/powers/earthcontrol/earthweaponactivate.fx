#############################################################
## EarthWeaponActivate.fx
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
		Bhvr	behaviors/EarthWeapon.bhvr
		Geom	FX_StoneMallet
	End

	Event
		EName 	Dust
		Type	local
		At	WepR
		Part1	:EarthWeaponDust.part
		part2	:EarthWeaponDust2.part
		Part2	:WeaponRocks04a.part
		Part3	:WeaponRocks03a.part
	End
End

#############################################################

End