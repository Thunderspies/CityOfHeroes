#                                                          
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			Sound BR_CuttingBeam_Attack_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			At C_Chest
			Type Local
			EName WeaponOffset
		End

		Event
			At WeaponOffset
			Type Local
			EName HeadOffset
			Bhvr :Tracer_Fade.bhvr
		End

	End

	Condition
		On Time
		Time 0

		Event
			At T_Chest
			Type Start
			EName LookAtTarget
			LookAt HeadOffset
		End

		Event
			At HeadOffset
			Type Local
			EName LaserFlash
			LifeSpan 30
		End

		Event
			At HeadOffset
			Type Local
			EName AfterGlowAnchor
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 23

		Event
			At WeaponOffset
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :BeamCharge_Arcs_Medium.part
			LifeSpan 20
		End

		Event
			At WeaponOffset
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Muzzle_Glow_Core_Fast.part
			LifeSpan 15
		End

		Event
			At WeaponOffset
			Type Local
			PMagnet BeamEnd
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0 90 0
			End
			Part :MuzzleFlash_Gas03.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 25

		Event
			At LookAtTarget
			Type Local
			EName SweepTargetL
			BhvrOverride
				PositionOffset 12 0 0
				InitialVelocity -1.5 0.0 0.0
			End
			LifeSpan 18
		End

		Event
			At LookAtTarget
			Type Local
			EName SweepTargetL2
			BhvrOverride
				PositionOffset 12 0 0
				InitialVelocity -1.5 0.0 0.0
			End
			LifeSpan 16
		End

		Event
			At SweepTargetL
			Type Local
			EName LaserSweepFlareL
			BhvrOverride
				PositionOffset 1.8 0 2
			End
			Part :Beam_HitTrail.part
			Part :Beam_Flare1.Part
			LifeSpan 30
		End

		Event
			At SweepTargetL
			Type Local
			EName LaserSweepL
			POther HeadOffset
			Part :Tracer_Trail05.part
			Part :Beam_Small_Continuous.part
			Part :Beam_Small_Continuous_Add.part
			Part :Beam_Small_Continuous_Electricity.part
			Part :Beam_Small_Continuous_Electricity_Blur.part
			Part :Beam_Flare03.part
			LifeSpan 18
		End

	End

	Condition
		On Cycle
		Time 1

		Event
			At SweepTargetL2
			Type Start
			POther HeadOffset
			Part :Beam_Small_AfterGlow.part
			LifeSpan 10
		End

	End

End
