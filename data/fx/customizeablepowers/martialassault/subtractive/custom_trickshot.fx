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
	Time 	17

	Event
		EName	Prime
		Type	startPositOnly
		At	hookup
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			TintGeom 1
		End
		Geom	Tintable_GEO_WepR_Shuriken
		Part1	CustomizablePowers_Pools\Epic_WeaponMastery\Subtractive\ShurikenStreaks01.part
		Magnet	Target
		LookAt	Target
	End

End


Condition
	On 	Time
	Time 	23

	Event
		Ename LstreakAnchor
		Type local
		At Chest
		BhvrOverride
			PositionOffset -1 0 0
		end
	End

	Event
		EName	Streak
		Type	PositOnly
		At	LstreakAnchor
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		BhvrOverride
			Scale	0.6 0.6 0.6
			PyrRotate 0 55 0
		End
		Sound clawtwirl5 100.0 100.0 .7
	End

End


Condition
	On 	Time
	Time 	28

	Event
		EName	streak
		Type	Destroy
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

End
##################################

