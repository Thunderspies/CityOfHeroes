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
	On	Time
	Time	5

	Event
		Ename 	InitStar
		Type	Local
		At	WepR
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			PositionOffset 0.5 0 0
			Spin 0 0 0
			PyrRotate 0 45 45
			TintGeom 1
		End
		Geom	Tintable_GEO_WepR_Shuriken
		Lifespan 29
	End
End

Condition
	On	Time
	Time	27

	Event
		Ename 	InitStarFlame
		Type	PositOnly
		At	InitStar
		BhvrOverride
			PositionOffset 0.5 0 0
		End
		Part	:ExplosiveCharge_Trail.part
		Lifespan 6
	End
End


Condition
	On 	Time
	Time 	35

	Event
		EName	roothookup
		Type	local
		At	root
		Bhvr	behaviors/alpha.bhvr
		Geom	RootToChestAdjustment
	End

	Event
		EName	hookup
		Type	local
		At	roothookup
		altpiv  1
		Geom	LungeNode
	End

	Event
		EName	Prime
		Type	start
		At	hookup
		altpiv	2
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			InitialVelocity		0 0 -0.3
			InitialVelocityJitter	0 0.3 0
			TintGeom 1
		End
		Geom	Tintable_GEO_WepR_Shuriken
		Part1	CustomizablePowers_Pools\Epic_WeaponMastery\Subtractive\ShurikenStreaks01.part
		Part	:ExplosiveCharge_Trail.part

		Magnet	Target
		LookAt	Target
	End

End


Condition
	On 	Time
	Time 	34

	Event
		EName	Streak
		Type	local
		At	Mystic
		#Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound clawtwirl5 100.0 100.0 .7
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		Ename	Boom
		Type	Local
		At	T_Chest
		ChildFX	:Custom_ExplosiveShuriken_Child.fx
	End

End

End
##################################

