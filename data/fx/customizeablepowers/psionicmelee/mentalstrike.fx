#
FxInfo

	LifeSpan 150


###########################################################
// Condition
// 	On 	Time
// 	Time	0
// 	Event
// 		Type	Local
// 		At	Root
// 		Sound 	PsionicMelee_MentalStrike 100 100 .8
// 	End
// End

	Condition
		On Time
		Time 1

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings01.part
			Part :PsioHeadRings02.part
			Part :PsioHeadGlow.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Lifespan 15
		End

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :EnergyCoreFlash.part
			Part :InfuseLight2.part
			Lifespan 18
		End
	End

	Condition
		On Time
		Time 5

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadFlash.part
			Lifespan 15
		End

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PsionicMelee\FistStrike.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset -1 0 0
				PyrRotate 0 180 0
			End
			Geom FX_GlowFist_HiRez_R
			LifeSpan 15
		End

	End

	Condition
		On time
		Time 05

		Event
			At HandR
			Type Local
			Bhvr CustomizeablePowers\PsionicMelee\FistStrike.bhvr
			Part :PsioHandFlash.part
			#Lifespan 1
		End
	End

	Condition
		On Time
		Time 12

		Event
			At LArmR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 2.7 0 0
			End
			Part CustomizeablePowers\PsionicMelee\FistGlow_Blur.part
			Part CustomizeablePowers\PsionicMelee\FistGlow.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 20

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PsionicMelee\FistStrike_Return.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset -1 0 0
				PyrRotate 0 180 0
			End
			Geom FX_GlowFist_HiRez_R
			LifeSpan 5
		End

	End

End
