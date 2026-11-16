#############################################################
## Death_AncientTablet_01.fx
#############################################################

FxInfo

LifeSpan 1150

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	RomanTabletBase
		Type	Start
		At	Root
		Geom	rmn_tablet_01_DMG_Base
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	DebrisOffset
		Type	start
		At	RomanTabletBase
		BhvrOverride
			PositionOffset	0 2 0
		End
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part
		LifeSpan 5
	End
End

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	start
		At	RomanTabletBase
		AltPiv	1
		BhvrOverride
			InitialVelocity		0.0 0.125 0.0
			InitialVelocityJitter	0.25 0.125 0.25
			SpinJitter		0.05 0.05 0.05
			physics 		1
			Drag			0.004
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris		1
			Gravity			0.048
		End
		Geom	rmn_tablet_01_DMG_P01
	End
End

Condition
	On	Time
	Time 	0

	Event
		Type	start
		At	RomanTabletBase
		AltPiv	2
		BhvrOverride
			InitialVelocity		0.0 0.125 0.0
			InitialVelocityJitter	0.25 0.125 0.25
			SpinJitter		0.05 0.05 0.05
			physics 		1
			Drag			0.004
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris		0
			Gravity			0.048
		End
		Geom	rmn_tablet_01_DMG_P02
	End
End

Condition
	On	Time
	Time 	0

	Event
		Type	start
		At	RomanTabletBase
		AltPiv	3
		BhvrOverride
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.125 0.25 0.125
			SpinJitter		0.05 0.05 0.05
			physics 		1
			Drag			0.004
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris		0
			Gravity			0.048
		End
		Geom	rmn_tablet_01_DMG_P03
	End
End

Condition
	On	Time
	Time 	0

	Event
		Type	start
		At	RomanTabletBase
		AltPiv	4
		BhvrOverride
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.125 0.25 0.125
			SpinJitter		0.05 0.05 0.05
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris		0
			Gravity			0.048
		End
		Geom	rmn_tablet_01_DMG_P04
	End
End

#############################################################

Condition
	Repeat	4
	Time 	0

	Event
		Type	start
		At	DebrisOffset
		BhvrOverride
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			SpinJitter		0.05 0.05 0.05
			Scale			0.25 0.25 0.25
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris		1
			Gravity			.048
		End
		Geom	rmn_tablet_01_DMG_P01
		Geom	rmn_tablet_01_DMG_P02
		Geom	rmn_tablet_01_DMG_P03
		Geom	rmn_tablet_01_DMG_P04
	End
End

#############################################################

Condition
	Repeat	12
	Time 	0

	Event
		HardwareOnly
		Type	start
		At	DebrisOffset
		BhvrOverride
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			SpinJitter		0.05 0.05 0.05
			Scale			0.125 0.125 0.125
			Drag			0.004
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris		1
			Gravity			.048
		End
		Geom	rmn_tablet_01_DMG_P01
		Geom	rmn_tablet_01_DMG_P02
		Geom	rmn_tablet_01_DMG_P03
		Geom	rmn_tablet_01_DMG_P04
	End
End

#############################################################

End