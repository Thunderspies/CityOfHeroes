#############################################################
## HurlBoulder_Attack.fx
#############################################################

FxInfo

Lifespan 200

Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	WepR
		Sound rockpickup7 100.0 100.0 0.85
	End
End

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	Root
		ChildFX	:Child_StoneFist_Activation.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	local
		At	WepR
		Part	CustomizeablePowers\StoneMelee\EarthHands.part
		Part	CustomizeablePowers\StoneMelee\EarthHands2.part
		part	CustomizeablePowers\StoneMelee\EarthHandsRocks.part
		lifespan 70
	End

	Event
		Type	local
		At	WepL
		Part	CustomizeablePowers\StoneMelee\EarthHands.part
		Part	CustomizeablePowers\StoneMelee\EarthHands2.part
		part	CustomizeablePowers\StoneMelee\EarthHandsRocks.part
		lifespan 70
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	start
		At	Mystic
		bhvr	Behaviors\CameraShakeBoulderHurl.bhvr
		Part	CustomizeablePowers\StoneMelee\EarthWeaponDustRocks2.part
		Part	CustomizeablePowers\StoneMelee\superStrengthDust1.part
		Part	CustomizeablePowers\StoneMelee\superStrengthDust2.part
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Mystic
		bhvr	CustomizeablePowers\StoneMelee\projectileHurlBolder.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_FX_Lava02
		Lifespan 37
	End

	Event
		Type	local
		At	Mystic
		bhvr	CustomizeablePowers\StoneMelee\projectileHurlBolder.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_FX_Lava02_AddGlow
		Lifespan 37
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	crack
		Type	start
		At	Mystic
		Part	CustomizeablePowers\StoneMelee\StalagtiteCrack4.part
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName	crack
		Type	start
		At	Mystic
		Part	CustomizeablePowers\StoneMelee\StalagtiteCrack5.part
	End
End

## PROJECTILE ######################################################

Condition
	On 	Time
	Time 	57 #87

	Event
		EName 	Prime
		Type	Start
		At	Mystic
		Bhvr	CustomizeablePowers\StoneMelee\projectileHurlBolder2.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_FX_Lava01
		Magnet	Target
		LookAt	Target
		Sound Rockthrow 100 100 0.9
	End

	Event
		EName 	Prime2
		Type	local
		At	Prime
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_FX_Lava01_AddGlow
	End
End

#############################################################

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
End

#############################################################

End