#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	MALE

	Event
		EName	MotionBlur_Male
		Type	Local
		At	Root
		ChildFX	:Child_PeircingRounds_MotionBlur_Male.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE

	Event
		EName	MotionBlur_Fem
		Type	Local
		At	Root
		ChildFX	:Child_PeircingRounds_MotionBlur_Fem.fx
	End
End

Condition
	On 	TriggerBits
	TriggerBits	HUGE

	Event
		EName	MotionBlur_Huge
		Type	Local
		At	Root
		ChildFX	:Child_PeircingRounds_MotionBlur_Huge.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PyrRotate	0 135 180
			Scale		0.2 0.2 0.2
			Alpha		255
		End
		Geom	Swoosh
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	WepR
		BhvrOverride
			PyrRotate	0 -135 180
			Scale		0.2 0.2 0.2
			Alpha		255
		End
		Geom	Swoosh
		Lifespan 28
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		sound	DW_PiercingRounds_01 120.0 100.0 1.0
	End
End

Condition
	On 	Time
	Time 	48

	Event
		EName	MuzzleFlashOffset
		Type	Local
		At	BendMystic
		BhvrOverride
			PositionOffset		0.75 0 0.375
		End
		ChildFX	WEAPONS\DualPistols\Child_PiercingRounds_VaporTrail.fx
	End

	Event
		Type	Start
		At	BendMystic
		BhvrOverride
			PyrRotate		0 -90 0
			Shake			0.5
			ShakeFallOff		0.05
			ShakeRadius		16
		End
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
		Lifespan 39
	End

	Event
		Type	Start
		At	WepR
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		ChildFX	WEAPONS\DualPistols\Child_ShellEject.fx
		Lifespan 1
	End
End

#############################################################

End