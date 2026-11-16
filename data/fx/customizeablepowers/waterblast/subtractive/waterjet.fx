#
FxInfo

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_WaterJet 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At WepL
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			LifeSpan 30
		End

		Event
			At WepR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type Local
			EName Anchor
			BhvrOverride
				PositionOffset 0 2.5 2
			End
			Part :Muzzle_Rings_Plasma.part
			Part :Muzzle_Glow_Core.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Chest
			Type Start
			EName Prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
			BhvrOverride
				Spin 0 0 0.25
				TrackRate 40
			End
			#Geom Geo_Fx_TestSphere
		End

		Event
			At Anchor
			Type Local
			EName MainBeam
			LookAt Target
			PMagnet Target
			POther Target
			Part :PreciseBeam09.part
			Part :Beam_Large.part
			Part :Beam_Large_Additive.part
			Part :BeamCharge_Large.part
			Part :Beam_Splashes.part
			LifeSpan 25
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At T_Chest
			Type Local
			ChildFx :WaterHit_Heavy.fx
		End

	End

End
