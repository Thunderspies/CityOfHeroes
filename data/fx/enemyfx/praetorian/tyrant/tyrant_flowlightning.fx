#
FxInfo

	Flags DontSuppress
	Lifespan 360

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ToV_LightninSibyl_FlowLightning_01 200 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			EName MidAnchor
			BhvrOverride
				PositionOffset 0 -165 0
			End
		End

		Event
			At Root
			Type Start
			EName Anchor
		End

		Event
			At Anchor
			Type Start
			EName TopAnchor
			LookAt Anchor
			POther Anchor
			BhvrOverride
				PositionOffset 0.0 170.0 0.0
			End
		End

		Event
			At Anchor
			Type StartPositOnly
			#Part :Projectile_Trail_Glow_Outer.part
		End

		Event
			At Anchor
			Type StartPositOnly
			LookAt TopAnchor
			POther TopAnchor
			#Part :Projectile_Trail_Flash.part
			#Part :Screen_Flash.part
			LifeSpan 15
		End

		Event
			At Anchor
			Type Start
			LookAt TopAnchor
			POther TopAnchor
			#Part :Projectile_Trail_Electricity_Warning.part
			#Part :Projectile_Trail_Electricity_Warning2.part
			LifeSpan 300
		End

		Event
			At Root
			Type Start
			Bhvr Events\EndGame\KeyesIsland\CameraShake_JumpSettle.bhvr
			BhvrOverride
				PositionOffset 0 6 0
			End
			#Part :OrbitalBeam_Flash.part
			Part :OrbitalBeam_ImpactFlash.part
			LifeSpan 120
		End

		Event
			At Root
			Type Local
			EName RingAnchor
			Bhvr Events\EndGame\KeyesIsland\Ground_Dust_Expand_Quick.bhvr
			BhvrOverride
				PositionOffset 0 3.5 0
			End
			Part :OrbitalBeam_Smoke_Ring_Expanding.part
			Part :OrbitalBeam_Fire_Ring_Expanding.part
			LifeSpan 270
		End

		Event
			At Root
			Type Start
			Part :OrbitalBeam_Shockwave.part
			LifeSpan 270
		End

		Event
			At Anchor
			Type StartPositOnly
			LookAt TopAnchor
			POther TopAnchor
			Part :Projectile_Trail_Core.part
			Part :Projectile_Trail_Core_Alpha.part
			Part :Base_Splash.part
			Part :Base_Splash_Core.part
			LifeSpan 240
		End

		Event
			At Anchor
			Type StartPositOnly
			LookAt TopAnchor
			POther TopAnchor
			Part :Projectile_Trail_Electricity.part
			Part :Projectile_Trail_Electricity2.part
			Part :Projectile_Trail_Glow.part
			LifeSpan 270
		End

		Event
			At Root
			Type StartPositOnly
			Part :Base_Splash_Flat.part
			Part :Base_Shockwaves_Flat.part
			Part :Base_Shockwave_Flat.part
			LifeSpan 285
		End

	End

End
