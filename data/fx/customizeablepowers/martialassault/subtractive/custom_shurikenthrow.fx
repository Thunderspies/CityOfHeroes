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
	Time 	0

	Event
		EName	hookup
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset 0 0 2
		End
	End
End

Condition
	On 	Time
	Time 	8

	Event
		EName	Prime
		Type	startPositOnly
		At	hookup
		Geom	Tintable_GEO_WepR_Shuriken
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			TintGeom 1
			Scale			2 2 2
			InitialVelocity		0 0 0
		End
		Part1	CustomizablePowers_Pools\Epic_WeaponMastery\Subtractive\ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Prime2
		Type	startPositOnly
		At	hookup
		Geom	Tintable_GEO_WepR_Shuriken
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			TintGeom 1
			StartJitter		0.5 0 0
			Scale			2 2 2
			InitialVelocity		0 0 0
			InitialVelocityJitter	0.5 0.5 0
		End
		Part1	CustomizablePowers_Pools\Epic_WeaponMastery\Subtractive\ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Prime3
		Type	startPositOnly
		At	hookup
		Geom	Tintable_GEO_WepR_Shuriken
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			TintGeom 1
			StartJitter		0.5 0 0
			Scale			2 2 2
			InitialVelocity		0 0 0
			InitialVelocityJitter	0.5 0.5 0
		End
		Part1	CustomizablePowers_Pools\Epic_WeaponMastery\Subtractive\ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target
	End

End


Condition
	On 	Time
	Time 	14.5

	Event
		Ename LstreakAnchor
		Type local
		At Chest
		BhvrOverride
			PositionOffset -1 0 0
		end
	End

	Event
		Ename RstreakAnchor
		Type local
		At Chest
		BhvrOverride
			PositionOffset 1 0 0
		end
	End

	Event
		EName	Streak
		Type	PositOnly
		At	LstreakAnchor
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		BhvrOverride
			Scale	0.5 0.5 0.5
			PyrRotate 0 45 0
		End
		Sound clawtwirl5 100.0 100.0 .7
	End

	Event
		EName	Streak2
		Type	PositOnly
		At	RstreakAnchor
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		BhvrOverride
			Scale	0.5 0.5 0.5
			PyrRotate 0 -45 180
		End
		Sound clawtwirl5 100.0 100.0 .7
	End

End


Condition
	On 	Time
	Time 	17.5

	Event
		EName	streak
		Type	Destroy
	End

	Event
		EName	streak2
		Type	Destroy
	End


End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	Prime3
		Type	Destroy
	End

End

End
##################################

