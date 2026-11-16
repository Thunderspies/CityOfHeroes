#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 800

#############################################################

Condition
	Event
		Type local
		At Root
		Sound	NovaFist_Attack_01 300 300 .9
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName	RockAnchor
		Type	StartPositOnly
		At	WepR
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	WepR
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Dust_Core_Flat.part
		Part	:Dust_Shockwave.part
		Part	:Light_Shockwave.part

		Lifespan	1
	End


End

Condition
	On 	Time
	Time 	34

	Event
		Type	StartPositOnly
		At	WepR
		bhvr    :Dust_Expanding.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Dust_Lingering.part
		Part	:Dust_Expanding.part
		Part	:Dust_Expanding_Thick.part
		Part	:Dust_Flash.part
		Part	:Dust_Shockwave_Fill.part
		Part	:SpeedLines.part
		Part	:Sparks.part
		Part	:Explosion_Smoke_Additive_Yellow.part
		Part	:Light_Flash.part
		Part	:Fire_Expanding.part

		Lifespan	60
	End

	Event
		EName	DustAnchor
		Type	StartPositOnly
		At	WepR
		BhvrOverride
			PositionOffset	0.0 4.0 0.0
		End
		Lifespan	20
	End

	Event
		Type	StartPositOnly
		At	WepR
		Part	:Dust_Pillar.part
		Part	:Fire_Pillar.part
		POther	DustAnchor
		Lifespan	1
	End

	Event
		EName	crack
		Type	Local
		At	Root
		Bhvr	CustomizeablePowers\SuperStrength\Splat_FootStomp.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 3.0
		End
		Splat	ImpactCracks.tga
		Lifespan 30
	End
End

Condition
	On 		Cycle
	Time 		1
	Repeat		1
	RepeatJitter	1
	Random		1

	Event
		Type	start
		At	RockAnchor
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0.0 0.5 0.0
			StartJitter		2.0 0.0 2.0
			Scale			1.0 1.0 1.0
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
		Lifespan 500
		LifespanJitter	90
	End

	Event
		Type	start
		At	RockAnchor
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0.0 0.5 0.0
			StartJitter		2.0 0.0 2.0
			Scale			1.0 1.0 1.0
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
		Geom	Earth_FXDebris02A
		Geom	Earth_FXDebris03A
		Geom	Earth_FXDebris05A
	#	CDestroy	1
		Lifespan 500
		LifespanJitter	90
	End
End
#############################################################

End