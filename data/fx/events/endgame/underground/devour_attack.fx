#############################################################
## Inferno.fx
#############################################################

FxInfo
LIfespan 300

#############################################################

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Root
		Sound 	Hamidon_Devour_Attack_01 100 100 .8
	End


	Event
		EName	RockAnchor
		Type	StartPositOnly
		At	HandL
		Bhvr	behaviors\CameraShake_Strong90.bhvr
		BhvrOverride
			PositionOffset	0 -10 0
		End
		Part	:Dust_Shockwave.part

		Lifespan	5
	End

	Event
		EName	RockAnchor
		Type	StartPositOnly
		At	HandR
		BhvrOverride
			PositionOffset	0 -10 0
		End
		Part	:Dust_Shockwave.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	RockAnchor
		Type	StartPositOnly
		At	Root
		Lifespan	10
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Dust_Lingering_Cloud.part
		Part	:Dust_Ground.part
		Lifespan	200
	End
End

Condition
	On	Time
	Time	30
	Repeat	2

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			StartJitter	15 -2.5 15
		End
		ChildFX	:CHILD_Devour_Attack.fx
		Lifespan	300
	End
End

Condition
	On	Time
	Time	37
	Repeat	2

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			StartJitter	20 -2.5 20
		End
		ChildFX	:CHILD_Devour_Attack.fx
		Lifespan	300
	End
End


Condition
	On	Time
	Time	44
	Repeat	2

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			StartJitter	25 -2.5 25
		End
		ChildFX	:CHILD_Devour_Attack.fx
		Lifespan	300
	End
End



Condition
	On 		Time
	Time 		37
	Repeat		15
	Random		1

	Event
		Type	StartPositOnly
		At	RockAnchor
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0.0 0.5 0.0
			StartJitter		30.0 0.0 30.0
			Scale			1.5 1.5 1.5
			InitialVelocity		0.0 2.0 0.0
			InitialVelocityJitter	0.5 0.5 0.5
			StartColor		255 240 192
			PyrRotateJitter		180 180 180
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End
		Geom	Earth_FXDebris01A
		CEvent	:CHILD_RockBreak.fx
		CDestroy	1
		Lifespan 	500
		LifespanJitter	2
	End
End

Condition
	On	Cycle
	Time	1
	Repeat		3
	RepeatJitter	2
	Event
		Type	StartPositOnly
		At	RockAnchor
		BhvrOverride
			PositionOffset		0.0 0.5 0.0
			StartJitter		30.0 0.0 30.0
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 2.0 0.0
			InitialVelocityJitter	0.5 0.5 0.5
			StartColor		255 240 192
			PyrRotateJitter		180 180 180
			Gravity			0.05
		End
		Geom	Earth_FXDebris02A
		Geom	Earth_FXDebris03A
		Geom	Earth_FXDebris05A
		Lifespan 500
		LifespanJitter	2
	End
End


#############################################################

End